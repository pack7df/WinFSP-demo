# Git Workflow - WinFSP Demo

This document describes the development standards and Git workflow for the WinFSP Demo project.

## 1. Primary Branches
- **main**: Contains stable, production-ready code and functional releases.
- **develop**: Integration branch where new features are merged before moving to main.

## 2. Feature Development
For every new improvement or change:
1. Create a branch from `develop`: `git checkout -b feature/feature-name`
2. Implement the functionality.
3. Use small, descriptive commits.
4. Merge into `develop` via Pull Request.

## 3. Commit Message Standards
Follow the format: `type: short description`
- `feat`: New feature.
- `fix`: Bug fix.
- `refactor`: Code cleanup or improvement without changing functionality.
- `docs`: Documentation changes.

## 4. Local Development Cycle
1. **Setup**: Open the project in Visual Studio 2022 (Open Folder).
2. **Build**: Ensure you are using `x64-Debug` or `x64-Release` configurations.
3. **Test**:
   - Stop any previous driver instances.
   - Run the executable with a drive letter (e.g., `Z:`).
   - Verify access using `dir Z:` in a terminal.

## 5. Pre-Push Checklist
Before pushing, ensure:
- The code compiles without errors or critical warnings.
- No binary files (`.exe`, `.pdb`) or output folders (`out/`, `build/`) are included.
- The `.gitignore` file is correctly configured to skip these automatically.
