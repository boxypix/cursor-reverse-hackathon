# Morphosis: Документация основных алгоритмов

## Обзор
Morphosis - это программа для генерации 3D визуализаций 4D фракталов Жюлиа с использованием алгоритма Marching Cubes для полигонизации.

## Архитектура алгоритмов

### 1. 4D Фракталы Жюлиа (Julia Sets)

#### Математическая основа
Фракталы Жюлиа в 4D пространстве используют кватернионы для представления комплексных чисел в четырех измерениях.

**Основная формула итерации:**
```
Z(n+1) = Z(n)² + C
```

Где:
- `Z` - кватернион (4D комплексное число): `z = x + yi + zj + wk`
- `C` - константа кватернион, определяющая тип фрактала
- Итерация продолжается до достижения максимального числа итераций или превышения порога

#### Реализация (`sample_julia.c`)

```c
float sample_4D_Julia(t_julia *julia, float3 pos)
{
    cl_quat z, c;
    uint iter = 0;
    
    // Инициализация кватерниона z из 3D позиции
    z.x = pos.x;  // Реальная часть
    z.y = pos.y;  // i компонента
    z.z = pos.z;  // j компонента  
    z.w = julia->w; // k компонента (константа)
    
    // Итеративный процесс
    while (iter < julia->max_iter) {
        z = cl_quat_mult(z, z);      // z²
        z = cl_quat_sum(z, julia->c); // z² + C
        
        float magnitude = cl_quat_mod(z);
        if (magnitude > 2.0f)
            return 0.0f; // Точка вне множества
        iter++;
    }
    return 1.0f; // Точка принадлежит множеству
}
```

#### Кватернионная арифметика (`lib_complex.c`)

**Умножение кватернионов:**
```c
cl_quat cl_quat_mult(cl_quat q1, cl_quat q2)
{
    cl_quat res;
    res.x = q1.x*q2.x - q1.y*q2.y - q1.z*q2.z - q1.w*q2.w;
    res.y = q1.x*q2.y + q1.y*q2.x + q1.z*q2.w - q1.w*q2.z;
    res.z = q1.x*q2.z + q1.z*q2.x + q1.w*q2.y - q1.y*q2.w;
    res.w = q1.x*q2.w + q1.w*q2.x + q1.y*q2.z - q1.z*q2.y;
    return res;
}
```

**Модуль кватерниона:**
```c
TYPE cl_quat_mod(cl_quat q)
{
    cl_quat conjugate = cl_quat_conjugate(q);
    cl_quat product = cl_quat_mult(q, conjugate);
    return sqrt(product.x² + product.y² + product.z² + product.w²);
}
```

### 2. Пространственная дискретизация

#### Создание 3D сетки (`point_cloud.c`)

Пространство разбивается на регулярную 3D сетку:

```c
void create_grid(t_data *data)
{
    // Создание осей X, Y, Z
    subdiv_grid(p0.x, p1.x, step_size, grid.x);
    subdiv_grid(p0.y, p1.y, step_size, grid.y); 
    subdiv_grid(p0.z, p1.z, step_size, grid.z);
}
```

**Параметры по умолчанию:**
- Область: `[-1.5, 1.5]³`
- Шаг дискретизации: `0.05`
- Размер сетки: `60×60×60 = 216,000` точек

#### Определение вокселей

Каждый воксель (3D пиксель) состоит из 8 вершин куба:

```c
void define_voxel(t_fract *fract, float step)
{
    // 8 вершин куба относительно центра
    float zz[2] = {-step/2, step/2};
    float xx[4] = {-step/2, step/2, step/2, -step/2};
    float yy[4] = {step/2, step/2, -step/2, -step/2};
    
    // Генерация всех 8 комбинаций
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            voxel[n].dx = xx[j];
            voxel[n].dy = yy[j]; 
            voxel[n].dz = zz[i];
            n++;
        }
    }
}
```

### 3. Алгоритм Marching Cubes

#### Принцип работы (`polygonisation.c`)

Marching Cubes - это алгоритм для извлечения 3D поверхности из скалярного поля. Он работает путем анализа каждого вокселя и определения, как поверхность проходит через него.

**Основные этапы:**

1. **Определение конфигурации куба:**
```c
uint getCubeIndex(float *values, uint pos)
{
    uint cubeindex = 0;
    for (int i = 0; i < 8; i++) {
        if (values[pos + i])
            cubeindex |= (1 << i);
    }
    return cubeindex;
}
```

2. **Интерполяция вершин на рёбрах:**
```c
float3 interpolate(float3 p0, float3 p1, float v0, float v1)
{
    if (v0 == 1.0f) return p0;
    if (v1 == 1.0f) return p1;
    
    // Линейная интерполяция для нахождения точки пересечения
    float mu = (1.0f - v0) / (v1 - v0);
    return p0 + mu * (p1 - p0);
}
```

3. **Генерация треугольников:**
```c
float3** polygonise(float3 *positions, float *values, uint2 *pos, t_data *data)
{
    uint cubeindex = getCubeIndex(values, pos->x);
    
    // Проверка по таблице рёбер
    if (edgetable[cubeindex] == 0)
        return NULL; // Нет пересечений
    
    // Получение вершин на рёбрах
    float3 *vertlist = get_vertices(cubeindex, positions, values, pos->x);
    
    // Генерация треугольников по таблице треугольников
    while (tritable[cubeindex][i] != -1) {
        create_triangle(vertlist, cubeindex, i);
        i += 3;
    }
    
    return triangles;
}
```

#### Таблицы поиска (`look-up.h`)

Алгоритм использует предвычисленные таблицы:
- **edgetable[256]** - определяет, какие рёбра пересекает поверхность
- **tritable[256][16]** - определяет, какие треугольники создавать

### 4. Основной цикл генерации (`build_fractal.c`)

```c
void build_fractal(t_data *data)
{
    // Проход по всем вокселям в 3D сетке
    for (size_t z = 0; z < grid_size; z++) {
        printf("%zu/%.0f\n", z + 1, grid_size); // Прогресс
        
        for (size_t y = 0; y < grid_size; y++) {
            for (size_t x = 0; x < grid_size; x++) {
                
                // Вычисление значений для 8 вершин текущего вокселя
                for (int corner = 0; corner < 8; corner++) {
                    float3 pos = {
                        grid.x[x] + voxel[corner].dx,
                        grid.y[y] + voxel[corner].dy,
                        grid.z[z] + voxel[corner].dz
                    };
                    
                    // Вычисление принадлежности к фракталу
                    vertexval[i] = sample_4D_Julia(julia, pos);
                    vertexpos[i] = pos;
                    i++;
                }
                
                // Применение Marching Cubes к текущему вокселю
                float3 **new_triangles = polygonise(vertexpos, vertexval, &pos, data);
                
                // Добавление новых треугольников к общему массиву
                if (new_triangles) {
                    data->triangles = concatenate_triangles(new_triangles, data->triangles);
                }
            }
        }
    }
}
```

### 5. Экспорт в OBJ формат (`write_obj.c`, `obj.c`)

#### Структура OBJ файла
```
# Morphosis 4D Julia Set
mtllib fractal.mtl

# Вершины (vertices)
v 0.123456 -0.789012 0.345678
v -0.456789 0.123456 -0.789012
...

# Нормали (normals) 
vn 0.707107 0.707107 0.000000
vn -0.577350 0.577350 0.577350
...

# Грани (faces)
f 1//1 2//2 3//3
f 4//4 5//5 6//6
...
```

#### Алгоритм записи
1. **Запись вершин:** Каждая вершина треугольника записывается с координатами (x, y, z)
2. **Вычисление нормалей:** Для каждого треугольника вычисляется нормаль через векторное произведение
3. **Запись граней:** Каждый треугольник записывается как грань с индексами вершин и нормалей

### 6. Параметры и настройки

#### Параметры Julia Set
```c
typedef struct s_julia {
    cl_quat c;          // Константа C (определяет форму фрактала)
    float w;            // 4-я координата для начального z
    uint max_iter;      // Максимальное число итераций (по умолчанию: 6)
    float threshold;    // Порог сходимости (по умолчанию: 2.0)
} t_julia;
```

#### Параметры пространства
```c
typedef struct s_fract {
    float3 p0, p1;      // Границы области: [-1.5, 1.5]³
    float grid_length;  // Длина стороны: 3.0
    float grid_size;    // Количество делений: 60
    float step_size;    // Шаг дискретизации: 0.05
} t_fract;
```

### 7. Сложность алгоритмов

#### Временная сложность
- **Julia Set вычисления:** O(n³ × max_iter), где n = grid_size
- **Marching Cubes:** O(n³)
- **Общая сложность:** O(n³ × max_iter)

#### Пространственная сложность
- **Сетка значений:** O(n³) для хранения результатов Julia Set
- **Треугольники:** O(t), где t - количество сгенерированных треугольников
- **Общая сложность:** O(n³ + t)

#### Производительность (для сетки 60³)
- **Всего вокселей:** 216,000
- **Время выполнения:** ~10-30 секунд (зависит от параметров)
- **Память:** ~50-100 МБ
- **Выходные треугольники:** 10,000-100,000 (зависит от сложности фрактала)

## Возможности оптимизации

### Алгоритмические
1. **Адаптивная сетка** - увеличение разрешения только в областях с деталями
2. **Октодеревья** - иерархическая структура для быстрого отсечения пустых областей
3. **Кэширование** - сохранение вычисленных значений Julia Set

### Параллельные вычисления
1. **OpenMP** - распараллеливание циклов по вокселям
2. **CUDA/OpenCL** - вычисления на GPU
3. **SIMD** - векторизация кватернионных операций

### Алгоритм Marching Cubes
1. **Marching Tetrahedra** - альтернативный алгоритм без неоднозначностей
2. **Dual Contouring** - лучшее сохранение острых углов
3. **Adaptive Marching Cubes** - переменное разрешение

## Заключение

Morphosis реализует классический pipeline для генерации 3D поверхностей из 4D математических объектов:
1. Математическая модель (4D Julia Sets с кватернионами)
2. Пространственная дискретизация (регулярная 3D сетка)
3. Извлечение поверхности (Marching Cubes)
4. Экспорт результата (OBJ формат)

Архитектура позволяет легко заменять отдельные компоненты и добавлять новые типы фракталов или алгоритмы полигонизации.
