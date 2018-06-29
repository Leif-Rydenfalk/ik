#include "Python.h"
#include "ik/ik.h"
#include "ik/python/ik_module_log.h"

/* ------------------------------------------------------------------------- */
static void
module_free(void* x)
{
    (void)x;
    ik.log.deinit();
}

/* ------------------------------------------------------------------------- */
static PyModuleDef ik_module_log = {
    PyModuleDef_HEAD_INIT,
    "log",                   /* Module name */
    NULL,                    /* docstring, may be NULL */
    -1,                      /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables */
    NULL,                    /* module methods */
    NULL,                    /* m_reload */
    NULL,                    /* m_traverse */
    NULL,                    /* m_clear */
    module_free              /* m_free */
};

/* ------------------------------------------------------------------------- */
static int
add_module_constants(PyObject* m)
{
    if (PyModule_AddStringConstant(m, "version", ik.info.version()) == -1) return -1;
    if (PyModule_AddIntConstant(m, "build_number", ik.info.build_number()) == -1) return -1;
    if (PyModule_AddStringConstant(m, "host", ik.info.host()) == -1) return -1;
    if (PyModule_AddStringConstant(m, "date", ik.info.date()) == -1) return -1;
    if (PyModule_AddStringConstant(m, "commit", ik.info.commit()) == -1) return -1;
    if (PyModule_AddStringConstant(m, "compiler", ik.info.compiler()) == -1) return -1;
    if (PyModule_AddStringConstant(m, "cmake", ik.info.cmake()) == -1) return -1;
    if (PyModule_AddStringConstant(m, "all", ik.info.all()) == -1) return -1;

    return 0;
}

/* ------------------------------------------------------------------------- */
PyObject* 
ik_module_log_create(void)
{
    PyObject* m;

    if (ik.log.init() != 0)
        goto ik_log_init_failed;

    m = PyModule_Create(&ik_module_log);
    if (m == NULL)
        goto module_alloc_failed;

    if (add_module_constants(m) != 0)
        goto init_module_failed;

    return m;

    init_module_failed  : Py_DECREF(m);
    module_alloc_failed : ik.log.deinit();
    ik_log_init_failed  : return NULL;
}