
#include <Python.h>
#include <math.h>
#include "structmember.h"

typedef struct {
    PyObject_HEAD
    double Re;
    double Im;
} Complexity;

static PyObject * Complexity_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    Complexity *self;
    self = (Complexity *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->Re = 0;
        self->Im = 0;
    }
    return (PyObject *)self;
}

static int Complexity_init(Complexity *self, PyObject *args, PyObject *kwds) {
    static char *kwlist[] = {"Re", "Im", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "dd", kwlist,
                                      &self->Re, &self->Im))
        return -1;
    return 0;
}

static PyMemberDef Complexity_members[] = {
    {"Re", T_DOUBLE, offsetof(Complexity, Re), 0,
     "Real"},
    {"Im", T_DOUBLE, offsetof(Complexity, Im), 0,
     "Imaginary"},
    {NULL}  /* Sentinel */
};

static PyObject* Complexity_arg(Complexity* self);
static PyObject* Complexity_exp(Complexity* self);
static PyObject* Complexity_log(Complexity* self);
static PyObject* Complexity_toString(Complexity* self);
static PyObject* Complexity_plus(Complexity* self, Complexity* another);
static PyObject* Complexity_minus(Complexity* self, Complexity* another);
static PyObject* Complexity_multiply(Complexity* self, Complexity* another);
static PyObject* Complexity_module(Complexity* self);
static PyObject* Complexity_invert(Complexity* self);
static PyObject* Complexity_negative(Complexity* self);
static PyObject* Complexity_divide(Complexity* self, Complexity* another);

// Functions

static PyObject * Complexity_sin(Complexity* self) {
    PyTypeObject *type = Py_TYPE(self);
    Complexity* a1 = (Complexity*)type->tp_alloc(type, 0);
    a1->Re = sin(self->Re) * cosh(self->Im);
    a1->Im = cos(self->Re) * sinh(self->Im);
    return (PyObject*)a1;
}

static PyObject * Complexity_cos(Complexity* self) {
    PyTypeObject *type = Py_TYPE(self);
    Complexity* a1 = (Complexity*)type->tp_alloc(type, 0);
    a1->Re = cos(self->Re) * cosh(self->Im);
    a1->Im = -sin(self->Re) * sinh(self->Im);
    return (PyObject*)a1;
}

static PyObject * Complexity_tan(Complexity* self) {
    return (PyObject*)Complexity_divide(Complexity_sin(self), Complexity_cos(self));
}

static PyObject * Complexity_cot(Complexity* self) {
    return (PyObject*)Complexity_divide(Complexity_cos(self), Complexity_sin(self));
}

static PyObject * Complexity_arg(Complexity* self) {
    double arg = atan2(self->Im, self->Re);
    return PyFloat_FromDouble(arg);
}

static PyObject * Complexity_exp(Complexity* self) {
    PyTypeObject *type = Py_TYPE(self);
    Complexity* a1 = (Complexity*)type->tp_alloc(type, 0);
    a1->Re = exp(self->Re) * cos(self->Im);
    a1->Im = exp(self->Re) * sin(self->Im);
    return (PyObject*) a1;
}

static PyObject * Complexity_log(Complexity* self) {
    PyTypeObject *type = Py_TYPE(self);
    Complexity* a1 = (Complexity*)type->tp_alloc(type, 0);
    double module = sqrt(self->Re * self->Re + self->Im * self->Im);
    double argument = atan2(self->Im, self->Re);
    a1->Re = log(module);
    a1->Im = argument;
    return (PyObject*) a1;
}

static PyMethodDef Complexity_methods[] = {
    {"sin", (PyCFunction)Complexity_sin, METH_NOARGS, "Sin of complexity"},
    {"cos", (PyCFunction)Complexity_cos, METH_NOARGS, "Cos of complexity"},
    {"tan", (PyCFunction)Complexity_tan, METH_NOARGS, "Tan of complexity"},
    {"cot", (PyCFunction)Complexity_cot, METH_NOARGS, "Cotan of complexity"},
    {"arg", (PyCFunction)Complexity_arg, METH_NOARGS, "Arg of complexity"},
    {"exp", (PyCFunction)Complexity_exp, METH_NOARGS, "Exp of complexity"},
    {"log", (PyCFunction)Complexity_log, METH_NOARGS, "Log of complexity"},
    {NULL}  /* Sentinel */
};

static PyObject * Complexity_toString(Complexity* self) {
    return PyUnicode_FromFormat("%S, %Si", PyFloat_FromDouble(self->Re), PyFloat_FromDouble(self->Im));
}

static PyObject * Complexity_plus(Complexity* self, Complexity* another) {
    PyTypeObject *type = Py_TYPE(self);
    Complexity* a1 = (Complexity*)type->tp_alloc(type, 0);
    a1->Re = self->Re + another->Re;
    a1->Im = self->Im + another->Im;
    return (PyObject*)(a1);
}

static PyObject * Complexity_minus(Complexity* self, Complexity* another) {
    PyTypeObject *type = Py_TYPE(self);
    Complexity* a1 = (Complexity*)type->tp_alloc(type, 0);
    a1->Re = self->Re - another->Re;
    a1->Im = self->Im - another->Im;
    return (PyObject*)a1;
}

static PyObject * Complexity_multiply(Complexity* self, Complexity* another) {
    PyTypeObject *type = Py_TYPE(self);
    Complexity* a1 = (Complexity*)type->tp_alloc(type, 0);
    a1->Re = (self->Re * another->Re) - (self->Im * another->Im);
    a1->Im = (self->Im * another->Re) + (self->Re * another->Im);
    return (PyObject*)a1;
}

static PyObject * Complexity_module(Complexity* self) {
    double module = sqrt(self->Re * self->Re + self->Im * self->Im);
    return PyFloat_FromDouble(module);
}

static PyObject * Complexity_invert(Complexity* self) {
    PyTypeObject *type = Py_TYPE(self);
    Complexity* a1 = (Complexity*)type->tp_alloc(type, 0);
    a1->Re = self->Re;
    a1->Im = -self->Im;
    return (PyObject*)a1;
}

static PyObject * Complexity_negative(Complexity* self) {
    PyTypeObject *type = Py_TYPE(self);
    Complexity* a1 = (Complexity*)type->tp_alloc(type, 0);
    a1->Re = -self->Re;
    a1->Im = -self->Im;
    return (PyObject*)a1;
}

static PyObject * Complexity_divide(Complexity* self, Complexity* another) {
    Complexity* invert_another = Complexity_invert(another);
    Complexity* a1 = Complexity_multiply(self, invert_another);
    double d = another->Re * another->Re + another->Im * another->Im;
    a1->Re /= d;
    a1->Im /= d;
    return (PyObject*)a1;
}

PyNumberMethods Complexity_as_number = {
     Complexity_plus, // binaryfunc nb_add,
     Complexity_minus, // binaryfunc nb_subtract,
     Complexity_multiply, // binaryfunc nb_multiply,
     0, // binaryfunc nb_remainder,
     0, // binaryfunc nb_divmod,
     0, // ternaryfunc nb_power,
     Complexity_negative, // unaryfunc nb_negative,
     0, // unaryfunc nb_positive,
     Complexity_module, // unaryfunc nb_absolute,
     0, // inquiry nb_bool,
     Complexity_invert, // unaryfunc nb_invert,
     0, // binaryfunc nb_lshift,
     0, // binaryfunc nb_rshift,
     0, // binaryfunc nb_and,
     0, // binaryfunc nb_xor,
     0, // binaryfunc nb_or,
     0, // unaryfunc nb_int,
     0, // void *nb_reserved,
     0, // unaryfunc nb_float,

     Complexity_plus, // binaryfunc nb_inplace_add,
     Complexity_minus, // binaryfunc nb_inplace_subtract,
     Complexity_multiply, // binaryfunc nb_inplace_multiply,
     0, // binaryfunc nb_inplace_remainder,
     0, // ternaryfunc nb_inplace_power,
     0, // binaryfunc nb_inplace_lshift,
     0, // binaryfunc nb_inplace_rshift,
     0, // binaryfunc nb_inplace_and,
     0, // binaryfunc nb_inplace_xor,
     0, // binaryfunc nb_inplace_or,

     0, // binaryfunc nb_floor_divide,
     Complexity_divide, // binaryfunc nb_true_divide,
     0, // binaryfunc nb_inplace_floor_divide,
     Complexity_divide, // binaryfunc nb_inplace_true_divide,

     0, // unaryfunc nb_index,

     0, // binaryfunc nb_matrix_multiply,
     0, // binaryfunc nb_inplace_matrix_multiply,
};

// Creating of module */

static PyTypeObject ComplexityType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "Complexity.Complexity",   /* tp_name */
    sizeof(Complexity),        /* tp_basicsize */
    0,                         /* tp_itemsize */
    0,                         /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
    0,                         /* tp_repr */
    &Complexity_as_number,     /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash  */
    0,                         /* tp_call */
    Complexity_toString,       /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,   /* tp_flags */
    "Complexity objects",      /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    Complexity_methods,        /* tp_methods */
    Complexity_members,        /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Complexity_init,      /* tp_init */
    0,                         /* tp_alloc */
    Complexity_new,                 /* tp_new */
};

static PyModuleDef Complexity2module = {
    PyModuleDef_HEAD_INIT,
    "Complexity",
    "Idk",
    -1,
    NULL, NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC
PyInit_Complexity(void)
{
    PyObject* m;

    if (PyType_Ready(&ComplexityType) < 0)
        return NULL;

    m = PyModule_Create(&Complexity2module);
    if (m == NULL)
        return NULL;

    Py_INCREF(&ComplexityType);
    PyModule_AddObject(m, "Complexity", (PyObject *)&ComplexityType);
    return m;
}
