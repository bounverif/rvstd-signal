#ifndef RVSTD_FORWARD_SIGNAL_HPP
#define RVSTD_FORWARD_SIGNAL_HPP

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <functional>
#include <memory>
#include <memory_resource>
#include <new>
#include <stdexcept>
#include <string>
#include <vector>

#define TypeT std::chrono::milliseconds
#define TypeV bool
#define AllocatorT std::allocator< std::pair< TypeT, TypeV > >
//#define AllocatorT std::pmr::polymorphic_allocator< std::pair< TypeT, TypeV > >()

namespace rvstd
{
   namespace forward_signal_bool
   {
      class forward_signal
      {
      private:
         std::vector< std::pair< TypeT, TypeV >, AllocatorT > data;  // Data buffer
         TypeV init = 0;

      public:
         forward_signal( std::initializer_list< std::pair< TypeT, TypeT > > common_, TypeV init_ = 0 );
         forward_signal( std::initializer_list< std::pair< int, int > > common_, TypeV init_ = 0 );
         explicit forward_signal() = default;
         explicit forward_signal( const TypeV& lval_init )
            : init( lval_init ){};

         ~forward_signal() = default;
         forward_signal( forward_signal&& ) noexcept = default;
         forward_signal& operator=( forward_signal&& ) noexcept = default;
         forward_signal( const forward_signal& ) = default;
         forward_signal& operator=( const forward_signal& ) = default;

         using size_type = std::size_t;

         void reserve( size_type n );
         auto capacity() const noexcept -> size_type;
         auto size() const noexcept -> size_type;
         bool empty() const noexcept;

         void append( const TypeT& pos, const TypeV& value );  // adds an element to end of the list
         const TypeV& at( const TypeT& pos );                  // returns value at a spesific time

         const TypeV& get_init() const;
         const std::vector< std::pair< TypeT, TypeV >, AllocatorT >& get_data_vector() const;
         void set_data_vector( const std::vector< std::pair< TypeT, TypeV >, AllocatorT >& data_ );
         const std::string to_string();

         forward_signal operator+( const forward_signal& other ) const;
         forward_signal operator&( const forward_signal& other ) const;
         forward_signal operator-( const forward_signal& other ) const;
         forward_signal operator not() const;
      };
   }  // namespace forward_signal_bool
}  // namespace rvstd

#endif  // RVSTD_FORWARD_SIGNAL_HPP