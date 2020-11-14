#include<iostream>
#include<gtest/gtest.h>

void test_algbase();
void test_algbase_default_para();
void test_allocator();
void test_ctad(); 
void test_decltype();
void test_declval();
void test_declval2();
void test_deleter();
void test_error_handling();
void test_filesystem();
void test_gtest(int argc, char* argv[]);
void test_hash();
void test_hash2();
void test_hashmap();
void test_hashmap2();
void test_ipc_unnamed_pipe();
void test_ipc_named_pipe(bool);
void test_ipc_shared_memory();
void test_literal_operator();
void test_memory();
void test_optional();
void test_parallel_algo();
void test_perfect_forwarding();
void test_placement_new0();
void test_placement_new1();
void test_placement_new2();
void test_pod();
void test_rand();
void test_sfinae0();
void test_sfinae1();
void test_sfinae2();
void test_sfinae_class();
void test_sfinae_concept();
void test_singleton();
void test_size();
void test_structured_binding();
void test_template_alias();
void test_template_member();
void test_template_template(); 
void test_template_common_type();
void test_template_zip_with();
void test_timer_resolution();
void test_tree();
void test_tuple();
void test_unique_ptr();
void test_unique_ptr_to_noncopyable();
void test_variadic();
void test_variant_and_any();
void test_variant_overloading_lambda();
void test_variant_aggregate_init();
void test_variant_CTAD();

// hackersrank
void test_anagram();
void test_int2str();
void test_matrix_spiral();

int main(int argc, char* argv[])
{    
    char* p = new char[10];
    p[12] = 'x';

/*  test_algbase();
    test_algbase_default_para(); 
    test_allocator();
    test_ctad();
    test_decltype();
    test_declval(); */ 
    test_declval2(); 
/*  test_deleter();
    test_error_handling();
    test_filesystem(); 
    test_gtest(argc, argv);
    test_hash();
    test_hash2(); 
    test_hashmap(); 
    test_hashmap2(); 
    test_ipc_unnamed_pipe();
    test_ipc_named_pipe(argc > 1); // with argument for producer
    test_ipc_shared_memory();
    test_literal_operator(); 
    test_memory(); 
    test_optional();  
    test_parallel_algo();
    test_perfect_forwarding(); 
    test_placement_new0();
    test_placement_new1();
    test_placement_new2(); 
    test_pod();
    test_rand();    
    test_sfinae0();
    test_sfinae1();
    test_sfinae2(); 
    test_sfinae_class(); 
    test_sfinae_concept();
    test_singleton();
    test_size(); 
    test_structured_binding(); 
    test_template_alias(); 
    test_template_member();
    test_template_template(); 
    test_template_common_type();
    test_template_zip_with(); 
    test_timer_resolution();
    test_tree(); 
    test_tuple();     
    test_unique_ptr(); 
    test_unique_ptr_to_noncopyable();
    test_variadic();  
    test_variant_and_any();  
    test_variant_overloading_lambda();
    test_variant_aggregate_init();
    test_variant_CTAD(); */

    // hackersrank
//  test_anagram();    
//  test_int2str();
//  test_matrix_spiral(); 
    std::cout << "\n\n\n";
    return 0;
}
