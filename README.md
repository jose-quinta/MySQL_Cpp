# MySQL_Cpp

CRUD de personas en consola usando C++ y MySQL Connector/C con prepared statements.

## Estructura

```
MySQL_Cpp/
├── include/
│   ├── db/             # Conexión a base de datos
│   │   └── Database.h
│   ├── model/          # Modelos de dominio
│   │   └── Person.h
│   ├── repository/     # Capa de acceso a datos
│   │   └── PersonRepository.h
│   └── ui/             # Interfaz de usuario
│       └── Console.h
├── src/
│   ├── db/
│   │   └── Database.cpp
│   ├── model/
│   │   └── Person.cpp
│   ├── repository/
│   │   └── PersonRepository.cpp
│   ├── ui/
│   │   └── Console.cpp
│   └── main.cpp
├── lib/
│   └── libmysql.dll
├── CMakeLists.txt
├── Makefile
├── .gitignore
└── README.md
```

## Requisitos

- Compilador C++11 (MinGW, MSVC, etc.)
- MySQL Server 5.7+ con Connector/C
- Base de datos `person_test` con tabla:
  ```sql
  CREATE TABLE Customer (
      id INT AUTO_INCREMENT PRIMARY KEY,
      fullname VARCHAR(100),
      age INT
  );
  ```
- Usuario `c++` con contraseña `c++` y permisos en `person_test`

## Compilar

### Make (MinGW):
```bash
make
```

### CMake:
```bash
mkdir build && cd build
cmake ..
make
```

Ejecutar:
```bash
bin/MySQL.exe
```

## Capas

| Capa | Directorio | Responsabilidad |
|---|---|---|
| **Database** | `db/` | Conexión RAII, ciclo de vida de la conexión |
| **Model** | `model/` | Entidad Person con lógica de dominio |
| **Repository** | `repository/` | CRUD con prepared statements |
| **UI** | `ui/` | Menú interactivo por consola |
