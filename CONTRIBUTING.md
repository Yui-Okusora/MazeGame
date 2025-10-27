# Coding Conventions

- Always enclose code in functions and objects.
- No global variables.
- External libraries should be put in libs/.
- Header files (.hpp) should contain declarations, inline functions, and templates.
- Source files (.cpp) should contain implementations.
- Include only what you need.
- Include your own headers first.
- Always include guard in header files (#pragma once)
- No circular dependencies.
- Avoid 'using namespace' in header files.

---
- **Always comment and explain each methods / function / class (why it is there, what it does).**
---

# Naming Conventions

| Type                    | Convention                | Example                                 |
| ----------------------- | ------------------------- | --------------------------------------- |
| **Classes / Structs**   | PascalCase                | `MemoryPool`, `FileMapper`              |
| **Functions / Methods** | camelCase                 | `allocateBlock()`, `loadTexture()`      |
| **Variables**           | camelCase                 | `bufferSize`, `threadCount`             |
| **Member fields**       | prefix "m_" + camelCase   | `m_specs`, `m_handle`                   |
| **Constants**           | UPPER_SNAKE_CASE          | `MAX_BUFFER_SIZE`                       |
| **Namespaces**          | lowercase                 | `soramem`, `render`, `math`             |
| **Files**               | snake_case                | `memory_pool.cpp`, `memory_pool.hpp`    |
| **Templates**           | PascalCase + T prefix     | `template<typename T> class Vector;`    |
| **Enums**               | PascalCase + scoped enums | `enum class FileType { Binary, Text };` |

# Brackets Conventions

```
class Foo
{
  
};

void foo()
{

}
```
