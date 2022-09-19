#pragma once

// =================================================== //
// Church encoding to manage conditions inside macros. //
// =================================================== //

#define  c_true(a, b) a // church-true
#define c_false(a, b) b // church-false

// church-and
#define c_and(p, q) \
    p(q, p)

// church-or
#define c_or(p, q) \
    p(p, q)

// church-not
#define c_not(p) \
    p(c_false, c_true)

// church-xor
#define c_xor(p, q) \
    p(c_not(q), q)

// church-if
#define c_if(c, t, f) \
    c(t, f)

// ========================================================================= //
// Macros for binding/managing and reading church-values from normal values. //
// ========================================================================= //

// Bind a church-value to a normal value.
#define def_c_values(b, r) b, r

// Bind value as church-true.
#define as_c_true(v) def_c_values(c_true, v)

// Bind value as church-false.
#define as_c_false(v) def_c_values(c_false, v)

// Get church-bool value -- church value.
#define get_c_bvalue c_true // As a church binding is just a set of two, 
                            // this functions is literally church-true.

// Get church-raw value -- normal value.
#define get_c_rvalue c_false // As a church binding is just a set of two, 
                             // this functions is literally church-false.

// ============== //
// The Map macro. // Maps items with a macro.
// ============== //

// Macro to fool the compiler into recursive macro calls. 
#define omit_call

// Defining the exhaustive rule to not run into infinite recursiveness.
#define end(...)

// Flag to indicate when to call the above macro.
#define end_flag def_c_values(c_false, 0)

// Reduces up-to 64 times; increase if necessary.
#define n0_beta_reduct(args...) args
#define n1_beta_reduct(args...) n0_beta_reduct(n0_beta_reduct(args))
#define n2_beta_reduct(args...) n1_beta_reduct(n1_beta_reduct(args))
#define n3_beta_reduct(args...) n2_beta_reduct(n2_beta_reduct(args))
#define n4_beta_reduct(args...) n3_beta_reduct(n3_beta_reduct(args))
#define n5_beta_reduct(args...) n4_beta_reduct(n4_beta_reduct(args))
#define    beta_reduct(args...) n5_beta_reduct(args)

#define impl_n2_map(fmacro, c_value, element, args...) \
    c_if(c_value, fmacro, end)(element) c_if(c_value, impl_n1_map, end) omit_call (fmacro, args)

#define impl_n1_map(fmacro, c_value, element, args...) \
    c_if(c_value, fmacro, end)(element) c_if(c_value, impl_n2_map, end) omit_call (fmacro, args)

#define impl_n0_map(fmacro, args...) \
    impl_n1_map(fmacro, args, end_flag)

#define impl_map(fmacro, args...) \
    beta_reduct(impl_n0_map(fmacro, args))

// =============== //
// The LMap macro. // Maps items and appends a separator between them.
// =============== //

// Empty to append to the end if no valid argument.
#define empty

#define impl_n2_lmap(fmacro, separator, c_value, element, la_c_value, la_element, args...) \
    c_if(c_value, fmacro, end)(element) c_if(la_c_value, separator, empty) c_if(c_value, impl_n1_lmap, end) omit_call (fmacro, separator, la_c_value, la_element, args)

#define impl_n1_lmap(fmacro, separator, c_value, element, la_c_value, la_element, args...) \
    c_if(c_value, fmacro, end)(element) c_if(la_c_value, separator, empty) c_if(c_value, impl_n2_lmap, end) omit_call (fmacro, separator, la_c_value, la_element, args)

#define impl_n0_lmap(fmacro, separator, args...) \
    impl_n1_lmap(fmacro, separator, args, end_flag, end_flag) // Needs two end flags, as implementations uses a look-ahead.

#define impl_lmap(fmacro, separator, args...) \
    beta_reduct(impl_n0_lmap(fmacro, separator, args))

// Some default separators.
#define sep_comma     ,
#define sep_semicolon ;

// Use this for now.
#define $    as_c_true
#define map  impl_map
#define lmap impl_lmap

// TODO: HOW THE FUCK ? THERE'S NO WAY TO BOOTSTRAP IT, IS THERE?

// ================================================== //
// The Map macro (No need to call as_c_true on args). // 
// ================================================== //
// #define tct_end_flag c_false

// #define n1_to_true(element, args...) \
//     as_c_true(element) sep_comma n0_to_true omit_call (args)

// #define n0_to_true(element, args...) \
//     as_c_true(element) sep_comma n1_to_true omit_call (args)

// #define to_true(args...) \
//     n0_to_true(args, tct_end_flag)
