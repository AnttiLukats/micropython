
//#define DEBUG_printf xil_printf

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "py/nlr.h"
#include "py/compile.h"
#include "py/runtime.h"
#include "py/repl.h"

#include "py/gc.h"
#include "py/stackctrl.h"

#include "led.h"

mp_uint_t mp_verbose_flag = 0;


void do_str(const char *src, mp_parse_input_kind_t input_kind) {
    mp_lexer_t *lex = mp_lexer_new_from_str_len(MP_QSTR__lt_stdin_gt_, src, strlen(src), 0);
    if (lex == NULL) {
        return;
    }

    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        qstr source_name = lex->source_name;
        mp_parse_tree_t parse_tree = mp_parse(lex, input_kind);
        mp_obj_t module_fun = mp_compile(&parse_tree, source_name, MP_EMIT_OPT_NONE, true);
        mp_call_function_0(module_fun);
        nlr_pop();
    } else {
        // uncaught exception
        mp_obj_print_exception(&mp_plat_print, (mp_obj_t)nlr.ret_val);
    }
}



STATIC char heap[16384];

int main(int argc, char **argv) {

	argc = 0;

    xil_printf("\r\n\nTE0741 Micro Phyton\r\n\n");

    gc_init(heap, heap + sizeof(heap));

    mp_stack_set_limit(10240);

    led_init();

	mp_init();


//    mp_obj_t m = mp_obj_new_module(MP_QSTR_pyb);
//    mp_store_attr(m, MP_QSTR_led, mp_make_function_n(1, pyb_led));

	//xil_printf("mp_init \r\n");

//    do_str("print(1)", MP_PARSE_SINGLE_INPUT);
	do_str("for i in range(10):\n  print(i)", MP_PARSE_FILE_INPUT);
	do_str("a = 12345\nprint(a)", MP_PARSE_FILE_INPUT);

    do_str("print('hello world!', list(x+1 for x in range(10)), end='eol\\n')", MP_PARSE_SINGLE_INPUT);

    do_str("print('Hello')\n", MP_PARSE_SINGLE_INPUT);
    do_str("print(123)\n", MP_PARSE_SINGLE_INPUT);

    do_str("import pyb", MP_PARSE_SINGLE_INPUT);

    do_str("myled = pyb.LED(1)", MP_PARSE_SINGLE_INPUT);

    do_str("myled.on()", MP_PARSE_SINGLE_INPUT);

    //do_str("myled.off()", MP_PARSE_SINGLE_INPUT);

//    do_str("print(pyb.millis())", MP_PARSE_SINGLE_INPUT);




//    xil_printf("\r\n mp_done! \r\n");

    //do_str("a = 1", MP_PARSE_SINGLE_INPUT);
    //xil_printf("\r\n mp_done! \r\n");

    //do_str("print(a)", MP_PARSE_SINGLE_INPUT);


    xil_printf("\r\n*** mp_done ***\r\n");

    mp_deinit();

    while (1)
    	;
    return 0;
}

void gc_collect(void) {
}

mp_lexer_t *mp_lexer_new_from_file(const char *filename) {
    return NULL;
}

mp_import_stat_t mp_import_stat(const char *path) {
    return MP_IMPORT_STAT_NO_EXIST;
}

mp_obj_t mp_builtin_open(uint n_args, const mp_obj_t *args, mp_map_t *kwargs) {
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_KW(mp_builtin_open_obj, 1, mp_builtin_open);

void nlr_jump_fail(void *val) {
}

void NORETURN __fatal_error(const char *msg) {
    while (1);
}

#ifndef NDEBUG
void MP_WEAK __assert_func(const char *file, int line, const char *func, const char *expr) {
    printf("Assertion '%s' failed, at file %s:%d\n", expr, file, line);
    __fatal_error("Assertion failed");
}
#endif


void mp_hal_stdout_tx_strn_cooked(const char *str, mp_uint_t len) {
	int i;
	for (i=0;i < len;i++) xil_printf("%c", str[i]);

}

