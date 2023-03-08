/*=============================================================================
  Copyright (C) 2012 - 2023 Allied Vision Technologies.  All Rights Reserved.

  Redistribution of this file, in original or modified form, without
  prior written consent of Allied Vision Technologies is prohibited.

-------------------------------------------------------------------------------

  File:        ImageAcquisition.h

  Description: Functions used to prepare, start and stop the image acquisition.

-------------------------------------------------------------------------------

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED
  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF TITLE,
  NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR  PURPOSE ARE
  DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=============================================================================*/

#ifndef IMAGE_ACQUISITION_H_
#define IMAGE_ACQUISITION_H_

#include <VmbC/VmbCTypeDefinitions.h>

/**
 * \brief Prepares and starts the stream.
 *
 * \param[in] cameraHandle  Handle of the alreay opened camera which should be used for streaming
 *
 * \return An error code indicating success or the type of error that occurred.
*/
VmbError_t StartStream(const VmbHandle_t cameraHandle);

/**
 * \brief Stops the stream reverting the steps done during StartStream.
 *
 * \param[in] cameraHandle  Handle of the alreay streaming camera
 *
 * \return An error code indicating success or the type of error that occurred.
*/
VmbError_t StopStream(const VmbHandle_t cameraHandle);

#endif
