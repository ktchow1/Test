#include<iostream>
#include<gtest/gtest.h>

void test_algbase();
void test_allocator();
void test_backtrace();
void test_casting();
void test_casting_shared_ptr();
void test_concepts();
void test_cpoly(); 
void test_cstr(); 
void test_ctad(); 
void test_decltype();
void test_declval0();
void test_declval1();
void test_deleter();
void test_deleter_for_pool();
void test_enum();
void test_filesystem();
void test_function_array_arg();
void test_function_macro();
void test_gtest(int argc, char* argv[]);
void test_hashfct0();
void test_hashfct1();
void test_hashmap0();
void test_hashmap1();
void test_hashmap2_reference_wrapper();
void test_index_seq0();
void test_index_seq1();
void test_index_seq2();
void test_ipc_unnamed_pipe();
void test_ipc_named_pipe(bool);
void test_ipc_shared_memory();
void test_is_vector();
void test_literal_operator();
void test_move_semantics();
void test_new_operator();
void test_optional();
void test_parallel_algo();
void test_perfect_forwarding();
void test_placement_new0();
void test_placement_new1();
void test_placement_new2();
void test_pod();
void test_pod_string();
void test_pod_T();
void test_randgen();
void test_range_lib();
void test_reference();
void test_sfinae0();
void test_sfinae1();
void test_sfinae2();
void test_sfinae_class();
void test_sfinae_is_base_implement();
void test_sfinae_is_base_usage();
void test_singleton();
void test_STL_constraint0();
void test_STL_constraint1();
void test_STL_constraint2(); /*
void test_STL_constraint3();  */
void test_structured_binding();
void test_template_member();   
void test_template_member_ptr();   
void test_template_template(); 
void test_template_common_type();
void test_template_zip_with();
void test_template_traits();
void test_template_AFT();
void test_template_AFT_invocable();
void test_time();
void test_timer_resolution();
void test_tree_traverse();
void test_tuple();
void test_unique_ptr();
void test_unique_ptr_to_noncopyable();
void test_valgrind();
void test_variadic();
void test_variant_and_any();
void test_variant_overloading_lambda();
void test_variant_aggregate_init();
void test_variant_CTAD();
void test_yaml0();
void test_yaml1();

// hackersrank
void test_anagram();
void test_int2str();        // citadel
void test_matrix_spiral();  // citadel
void test_tree_checker();   // optiver

// psql
void test_psql0();
void test_psql1();
void test_psql_API();
void test_psql_tuple();

int main(int argc, char* argv[])
{    
//  test_psql0();
//  test_psql1();
    test_psql_API();
//  test_psql_tuple();

    bool run_all = false;
    if (run_all)
    {
        test_algbase();
        test_allocator();
    //  test_backtrace();
        test_casting();
        test_casting_shared_ptr();
        test_concepts();
        test_cpoly(); 
        test_cstr(); 
        test_ctad();
        test_decltype();
        test_declval0(); 
        test_declval1(); 
        test_deleter();
        test_deleter_for_pool();
        test_enum(); 
        test_filesystem(); 
        test_function_array_arg();
        test_function_macro();
        test_gtest(argc, argv);
        test_hashfct0();
        test_hashfct1(); 
        test_hashmap0(); 
        test_hashmap1(); 
        test_hashmap2_reference_wrapper();
        test_index_seq0();
        test_index_seq1();
        test_index_seq2();
    //  test_ipc_unnamed_pipe();
    //  test_ipc_named_pipe(argc > 1); // with argument for producer
    //  test_ipc_shared_memory();
        test_is_vector();
        test_literal_operator(); 
        test_move_semantics(); 
        test_new_operator(); 
        test_optional();  
        test_parallel_algo();
        test_perfect_forwarding(); 
        test_placement_new0();
        test_placement_new1();
        test_placement_new2(); 
        test_pod();
        test_pod_string();
        test_pod_T();
        test_randgen();    
        test_range_lib();    
        test_reference();
        test_sfinae0();
        test_sfinae1();
        test_sfinae2(); 
        test_sfinae_class(); 
        test_sfinae_is_base_implement();
        test_sfinae_is_base_usage();
        test_singleton();
        test_STL_constraint0();
        test_STL_constraint1();
        test_STL_constraint2();
    //  test_STL_constraint3();
        test_structured_binding(); 
        test_template_member();
        test_template_member_ptr();
        test_template_template(); 
        test_template_common_type();
        test_template_zip_with(); 
        test_template_traits();
        test_template_AFT();
        test_template_AFT_invocable();
        test_time();
    //  test_timer_resolution();
        test_tree_traverse(); 
        test_tuple();     
        test_unique_ptr(); 
        test_unique_ptr_to_noncopyable();
        test_valgrind();  
        test_variadic();  
        test_variant_and_any();  
        test_variant_overloading_lambda();
        test_variant_aggregate_init();
        test_variant_CTAD(); 
        test_yaml0(); 
        test_yaml1(); 
    }

    // hackersrank
//  test_anagram();    
//  test_int2str();
//  test_matrix_spiral(); 
//  test_tree_checker();
    std::cout << "\n\n\n";
    return 0;
}
