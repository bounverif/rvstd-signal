#ifndef INTERVAL_SET_RVSTD
#define INTERVAL_SET_RVSTD

#include <memory>
#include <memory_resource>
#include <limits>
#include <vector>
#include <tuple>
#include <string>
#include <iterator>

namespace rvstd {
    
class interval;
class interval_set;


template < class TypeT = double, class AllocatorT >
class interval_set {

private:
    
    TypeT low_limit, high_limit; // Maximum number limits
    vector < TypeT > data_ (256); // Data buffer
    std::unique_ptr ptr; // Pointer to the associated buffer.
    AllocatorT allocator; // Custom allocator

    std::iterator iterator_ = data_.iterator; // Iterator
    interval< TypeT, AllocatorT > iterate(); // Calculates the real interval object boundaries and returns those.

    // Because the std::vector is direct accessible by index, we may eliminate iterator. Placed here for emergency.
    
public:
    
    explicit interval_set(
        TypeT low_limit, 
        TypeT high_limit
    ); // Single point initialization
    ~interval_set(); // Default destructor
    explicit interval_set (const interval_set&); // Default copy constructor
    explicit interval_set& operator= (const interval_set&); // Copy assignment
    explicit interval_set (interval_set&&); // Move constructor
    explicit interval_set& operator= (interval_set&&); // Move assignment

    bool contains(const interval < TypeT, AllocatorT >& );

    interval< TypeT, AllocatorT > find(const TypeT); // finds and returns specific interval.
    void erase(); // Remove completely.
    void delete_interval(int pos); // used to remove posth interval. Can be used to delete first element. Left to right order.
    void delete_interval(std::tuple < TypeT, TypeT >); //delete specific interval.
    bool is_empty(); // TRIVIAL

    
    std::vector < interval < TypeT, AllocatorT > get_intervals(); // Returns all intervals in a vector by reading data_.

    explicit std::unique_ptr& operator new (AllocatorT& allocator);
    explicit std::unique_ptr& operator delete (AllocatorT& allocator);

    explicit interval_set < TypeT, AllocatorT >& operator not ();

    explicit interval_set < TypeT, AllocatorT >& operator+ (const interval_set & other);
    explicit interval_set < TypeT, AllocatorT >& operator- (const interval_set & other);
    explicit interval_set < TypeT, AllocatorT >& operator& (const interval_set & other);
    
    explicit interval_set < TypeT, AllocatorT >& operator+ (interval_set & interval);
    explicit interval_set < TypeT, AllocatorT >& operator- (interval_set & interval);
    explicit interval_set < TypeT, AllocatorT >& operator& (interval_set & interval);
    
    
    TypeT get_domain(); // returns the type of interval domain type.
    AllocatorT get_allocator_type(); // returns the type of allocator.
    std::unique_ptr get_unique_ptr(); // returns a pointer
    
    // UTILITY
    
    std::string to_string();
    
    
}

template < class TypeT = double, class AllocatorT > 
class interval {
    
public:
    
    explicit interval(TypeT low, TypeT high); // Default constructor
    ~interval(); // Default destructor
    explicit interval (const interval&); // Default copy constructor
    explicit interval& operator= (const interval&); // Copy assignment
    explicit interval (interval&&); // Move constructor
    explicit interval& operator= (interval&&); // Move assignment
    
    int set_boundaries(TypeT low, TypeT high);
    
    std::tuple< TypeT, TypeT > get_values();
    std::tuple< TypeT, TypeT > get_boundaries();
    
    TypeT get_type();
    AllocatorT get_allocator_type();
    std::unique_ptr get_unique_ptr();
    
    
    
private:

    TypeT low, high;
    TypeT lower_bound, upper_bound;
    std::unique_ptr ptr; // Pointer to the associated buffer.
    AllocatorT allocator; // Custom allocator

}

}
    
    
#endif // INTERVAL_SET_RVSTD
    
    
    
    
    
    
    
    
