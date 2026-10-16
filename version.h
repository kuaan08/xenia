#pragma once

// Portable fallback build metadata for the CMake / UWP project.
// The original Xenia/Premake build generates version.h separately. The
// UWP Visual Studio project does not invoke that generator, so an absent
// header made the Xbox build fail with C1083.
//
// Define these macros on the compiler command line when a release build
// needs an exact branch, revision or date. Without those overrides, local
// builds remain self-contained and compile successfully.
#ifndef XE_BUILD_BRANCH
#define XE_BUILD_BRANCH "xenia-canary-uwp"
#endif
#ifndef XE_BUILD_COMMIT
#define XE_BUILD_COMMIT "unknown"
#endif
#ifndef XE_BUILD_COMMIT_SHORT
#define XE_BUILD_COMMIT_SHORT "local"
#endif
#ifndef XE_BUILD_DATE
#define XE_BUILD_DATE __DATE__
#endif
