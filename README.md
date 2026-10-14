# DeepFX-Development-Kit

Repository: Deep FX Development Kit — collection of subprojects, tools and sample code for the DeepFX suite.

Detected repository layout
- Visual Studio solution(s) and project directories (files like `DeepFX Development Kit.sln`, `Deep Fx Development Kit.smproj`).
- Multiple subdirectories containing components: `Deep Fx Core`, `Deep FX Studio`, `Deep FX Studio App`, `Deep Fx Studio OpenVDB Plugin`, `Deep Fx FluentCL`, `Deep FX Node Editor`, and others.

Primary language(s)
- C++ / Visual Studio projects (solution files present). Some projects may include C#/.NET solution/project files.

What this repository contains
- A development kit that groups multiple DeepFX-related projects, plugins and example applications.
- Native libraries, tools, and Visual Studio solution files to build and run the components on Windows.

Quick start (Windows / Visual Studio)
1. Clone the repository:

   git clone https://github.com/mambobanda/DeepFX-Development-Kit.git

2. Open `DeepFX Development Kit.sln` (or the relevant `.sln` / `.smproj` for the component you want) in Visual Studio (Visual Studio 2017/2019/2022 recommended).
3. Restore any required third-party dependencies (external libs) used by the projects. Check each subproject's README (if present) or project files for references to external libraries.
4. Build the solution (select desired configuration: Release/Debug and platform x64/x86 as appropriate).

Building on other platforms
- These projects are structured as Visual Studio solutions. Cross-platform building may require porting to CMake or another build system and resolving platform-specific dependencies.

Testing
- Some projects include unit-tests or example apps (look for `VFS_UNIT_TEST`, `vfs_unit_test.cpp`, or test solution folders). Run the test projects from Visual Studio's Test Explorer or build and run the corresponding test executable.

Recommended next steps
- Add per-subproject README files inside each subdirectory describing build steps and dependencies specific to that component (plugins, installers, OpenVDB plugin, etc.).
- Add a LICENSE file if you intend to publish under a specific license.

Contact / Notes
- This README was generated automatically by a repository scan. Please update the instructions below with specific build steps, dependency versions, and project-level notes.
