# Flujo de Trabajo Git - WinFSP Demo

Este documento describe el estándar de desarrollo y el flujo de trabajo Git para el proyecto SaunaFS WinFSP Demo.

## 1. Ramas Principales
- **main**: Contiene el código estable y versiones funcionales.
- **develop**: Rama de integración donde se combinan las nuevas funcionalidades antes de pasar a main.

## 2. Desarrollo de Funcionalidades (Features)
Para cada nueva mejora o cambio:
1. Crear una rama desde `develop`: `git checkout -b feature/nombre-mejora`
2. Trabajar en la funcionalidad.
3. Realizar commits pequeños y descriptivos.
4. Fusionar en `develop` mediante Pull Request o Merge.

## 3. Estándar de Commits
Se recomienda seguir el formato: `tipo: descripción corta`
- `feat`: Nueva funcionalidad.
- `fix`: Corrección de errores.
- `refactor`: Limpieza o mejora de código sin cambiar funcionalidad.
- `docs`: Cambios en documentación.

## 4. Ciclo de Desarrollo Local
1. **Configurar**: Abrir en Visual Studio 2022 (Abrir Carpeta).
2. **Compilar**: Asegurarse de usar la configuración `x64-Debug` o `x64-Release`.
3. **Probar**:
   - Detener cualquier instancia previa del driver.
   - Lanzar el ejecutable pasando la letra de unidad (ej: `Z:`).
   - Verificar con `dir Z:` en una terminal.

## 5. Limpieza antes de Push
Antes de realizar un push, asegúrate de:
- Que el código compile sin errores ni advertencias graves.
- No incluir archivos binarios (`.exe`, `.pdb`) ni carpetas de salida (`out/`, `build/`).
- El archivo `.gitignore` ya está configurado para evitar esto automáticamente.
