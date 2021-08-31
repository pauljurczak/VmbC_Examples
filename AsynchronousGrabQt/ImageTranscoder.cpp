#include <algorithm>
#include <limits>
#include <thread>
#include <type_traits>

#include "AcquisitionManager.h"
#include "Image.h"
#include "ImageTranscoder.h"
#include "VmbException.h"

#include <QImage>
#include <QPixmap>

#include "VimbaC/Include/VimbaC.h"

namespace VmbC
{
    namespace Examples
    {
        ImageTranscoder::ImageTranscoder(AcquisitionManager& manager)
            : m_acquisitionManager(manager)
        {
        }

        void ImageTranscoder::PostImage(VmbHandle_t const streamHandle, VmbFrameCallback callback, VmbFrame_t const* frame)
        {
            bool notify = false;

            if (frame != nullptr && frame->receiveStatus == VmbFrameStatusComplete && (frame->receiveFlags & VmbFrameFlagsDimension) == VmbFrameFlagsDimension)
            {

                auto message = std::make_unique<TransformationTask>(streamHandle, callback, *frame);

                {
                    std::lock_guard<std::mutex> lock(m_inputMutex);
                    if (m_terminated)
                    {
                        message->m_canceled = true;
                    }
                    else
                    {
                        m_task = std::move(message);
                        notify = true;
                    }
                }
            }

            if (notify)
            {
                m_inputCondition.notify_one();
            }
        }

        void ImageTranscoder::Start()
        {
            {
                std::lock_guard<std::mutex> lock(m_inputMutex);
                if (!m_terminated)
                {
                    throw VmbException("ImageTranscoder is still running");
                }
                m_terminated = false;
            }
            m_thread = std::thread(&ImageTranscoder::TranscodeLoop, std::ref(*this));
        }

        void ImageTranscoder::Stop() noexcept
        {
            {
                std::lock_guard<std::mutex> lock(m_inputMutex);
                if (m_terminated)
                {
                    return;
                }
                m_terminated = true;
                if (m_task)
                {
                    m_task->m_canceled;
                    m_task.reset();
                }
            }
            m_inputCondition.notify_all();
            m_thread.join();
        }

        void ImageTranscoder::SetOutputSize(QSize size)
        {
            std::lock_guard<std::mutex> lock(m_sizeMutex);
            m_outputSize = size;
        }

        ImageTranscoder::~ImageTranscoder()
        {
            // tell the thread about the shutdown
            if (!m_terminated)
            {
                Stop();
            }
        }

        void ImageTranscoder::TranscodeLoopMember()
        {
            std::unique_lock<std::mutex> lock(m_inputMutex);

            while (true)
            {
                if (!m_terminated && !m_task)
                {
                    m_inputCondition.wait(lock, [this]() { return m_terminated || m_task; }); // wait for frame/termination
                }

                if (m_terminated)
                {
                    return;
                }

                {
                    // get task
                    decltype(m_task) task;
                    std::swap(task, m_task);

                    lock.unlock();

                    if (task)
                    {
                        try
                        {
                            TranscodeImage(*task);
                        }
                        catch (VmbException const&)
                        {
                            // todo?
                        }
                        catch (std::bad_alloc&)
                        {
                            // todo?
                        }
                    }

                    lock.lock();

                    if (m_terminated)
                    {
                        // got terminated during conversion -> don't reenqueue frames
                        task->m_canceled = true;
                        return;
                    }
                }
            }

        }

        void ImageTranscoder::TranscodeImage(TransformationTask& task)
        {
            constexpr auto targetFormat = VmbPixelFormatType::VmbPixelFormatArgb8; // equivalent of QImage::Format_RGB32

            VmbFrame_t const& frame = task.m_frame;

            Image const source(task.m_frame);

            // allocate new image, if necessary
            if (!m_transformTarget)
            {
                m_transformTarget = std::make_unique<Image>(targetFormat);
            }

            m_transformTarget->Convert(source);

            QImage qImage(m_transformTarget->GetData(),
                          m_transformTarget->GetWidth(),
                          m_transformTarget->GetHeight(),
                          m_transformTarget->GetBytesPerLine(),
                          QImage::Format::Format_RGB32);

            QPixmap pixmap = QPixmap::fromImage(qImage, Qt::ImageConversionFlag::ColorOnly);

            QSize size;
           
            {
                std::lock_guard<std::mutex> lock(m_sizeMutex);
                size = m_outputSize;
            }

            m_acquisitionManager.ConvertedFrameReceived(pixmap.scaled(size, Qt::AspectRatioMode::KeepAspectRatio));
        }

        void ImageTranscoder::TranscodeLoop(ImageTranscoder& transcoder)
        {
            transcoder.TranscodeLoopMember();
        }

        ImageTranscoder::TransformationTask::TransformationTask(VmbHandle_t const streamHandle, VmbFrameCallback callback, VmbFrame_t const& frame)
            : m_streamHandle(streamHandle),
            m_callback(callback),
            m_frame(frame)
        {
        }

        ImageTranscoder::TransformationTask::~TransformationTask()
        {
            if (!m_canceled)
            {
                VmbCaptureFrameQueue(m_streamHandle, &m_frame, m_callback);
            }
        }
    }
}