/**
 ******************************************************************************
 * Xenia Canary UWP frontend text translation (the guest language is separate)
 ******************************************************************************
 */
#ifndef XENIA_APP_FRONTEND_TEXT_H_
#define XENIA_APP_FRONTEND_TEXT_H_

namespace xe::app {
// English keys are also the fallback for languages not yet fully localized.
// Returned pointers have static storage duration, safe for ImGui labels.
const char* FrontendText(const char* english);
}  // namespace xe::app

#endif  // XENIA_APP_FRONTEND_TEXT_H_
