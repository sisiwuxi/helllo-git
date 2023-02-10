# virtual & override

- dynamic single-dispatch subtype polymorphism
- pros
  - decrease if_then_else

## reference
- http://blog.ezyang.com/

## key-value
- python dictionary[key, value]
  - key is unique and immutable

## C++ virtual table
- dispatch vtables are allocated per class
- only this pointer need
- vptr -> vtbl (meth1, meth2, ...)

## pytorch dispatch
- dispatch tables are allocated per operator(compute schedule)
- include tensor and other meta(context, dtype, backend...)
- the computation of dispatch key considers all arguments to the operator(multiple dispatch) as well as thread-local state(TLS)
- the dispatcher supports boxing and unboxing as part of the calling convention for operators
- DispatchKeySet[8]
  - sum
    - tensor inputs
      - cpu, gpu
    - local include
      - tracing
    - global
      - autograd
      - backend select  
  - sub
    - local exclude
- dispatch tables
  -       XLAPreAutograd, tracer, autograd, backendselect, private, sparse, quantize, xla, cuda, cpu
  - add       y             y        y            y           y        y       y       y     y    y 
  - mul       y             y        y            y           y        y       y       y     y    y 
  - dot
  - conv
  - ...
- register a kernel
  - TORCH_LIBRARY_IMPL(aten,cpu,m){m.impl("mul", cpu_mul)}
  - TORCH_LIBRARY_IMPL(aten,m){m.def("mul", catchall_mul)}
  - TORCH_LIBRARY_IMPL(_,backendselect,m){m.fallback(bs_fallback)}

---
---

# recursion
- problem -> sub-problem, until the subproblem becomes the simplest problem that cannot be split and can be solved directly.
- ending
  - basic case
- recurrence relation