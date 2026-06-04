# MySQL_Cpp

CRUD de personas con interfaz gráfica (FLTK) y MySQL/MariaDB Connector/C con prepared statements.

## Estructura

```
MySQL_Cpp/
├── include/
│   ├── db/Database.h              # Conexión MySQL con RAII
│   ├── model/Person.h             # Modelo de dominio
│   ├── repository/PersonRepository.h  # CRUD con prepared statements
│   └── ui/
│       ├── MainWindow.h           # Ventana principal (FLTK)
│       └── Console.h              # Interfaz de consola (alternativa)
├── src/
│   ├── db/Database.cpp
│   ├── model/Person.cpp
│   ├── repository/PersonRepository.cpp
│   ├── ui/
│   │   ├── MainWindow.cpp
│   │   └── Console.cpp
│   └── main.cpp
├── lib/
│   ├── libmysql.dll
│   └── libmariadb.dll
├── CMakeLists.txt
├── Makefile
├── .gitignore
└── README.md
```

## Requisitos

- Compilador C++11 (MinGW, MSVC, etc.)
- MySQL Server 5.7+ / MariaDB 10+ con Connector/C
- FLTK 1.4+ (biblioteca GUI)
- Base de datos `person_test` con tabla `Customer`:
  ```sql
  CREATE TABLE Customer (
      id INT AUTO_INCREMENT PRIMARY KEY,
      name VARCHAR(100),
      fLastname VARCHAR(100),
      mLastname VARCHAR(100),
      age INT
  );
  ```

## Compilar

### Con CMake:
```bash
mkdir build && cd build
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..
mingw32-make
```

Copiar la DLL de MariaDB al directorio de salida:
```bash
cp ../lib/libmariadb.dll .
```

Ejecutar:
```bash
MySQL_Cpp.exe
```

## Capas

| Capa | Directorio | Responsabilidad |
|---|---|---|
| **Database** | `db/` | Conexión RAII, ciclo de vida de la conexión |
| **Model** | `model/` | Entidad Person con lógica de dominio |
| **Repository** | `repository/` | CRUD con prepared statements |
| **UI** | `ui/` | Interfaz gráfica (FLTK) o consola |
