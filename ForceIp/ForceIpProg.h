/**
 * \date 2022
 * \copyright Allied Vision Technologies.  All Rights Reserved.
 *
 * \copyright Redistribution of this file, in original or modified form, without
 *            prior written consent of Allied Vision Technologies is prohibited.
 *
 * \warning THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF TITLE,
 * NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR  PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * \brief Declaration of the function which implements the force ip example
 */

#ifndef FORCE_IP_PROG_H_
#define FORCE_IP_PROG_H_

/**
 * \brief modifies the IP configuration for a camera identified by the given mac address
 *
 * Starts the VmbC API. Sends a force IP command to apply the given ip configuration.
 * The configuration will be lost after a power-cycle of the camera.
 *
 * \param[in] tl      If not NULL, perform the operation using a transport layer, otherwise use an interface
 * \param[in] mac     Mac address of the desired camera whose ip configuration is to be updated
 * \param[in] ip      The desired ip address
 * \param[in] subnet  The desired subnet mask
 * \param[in] gateway The desired gateway. Optional, can be 0.
 *
 * \return a code to return from main()
*/
int ForceIpProg(const char* const tl, const char* const strMAC, const char* const strIP, const char* const strSubnet, const char* const strGateway);

#endif // FORCE_IP_PROG_H_
