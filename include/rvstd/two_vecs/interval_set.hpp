#ifndef INTERVAL_SET_RVSTD
#define INTERVAL_SET_RVSTD

#include <chrono>
#include <climits>
#include <string>
#include <vector>

namespace rvstd
{
   namespace two_vecs
   {
      template< class TypeT = double, class AllocatorT = std::allocator< TypeT > >
      class interval_set
      {
      private:
         std::vector< TypeT > data;  // Data buffer
         std::vector< bool > values;

      public:
         using size_type = int;

         interval_set( std::initializer_list< TypeT > data_, std::initializer_list< bool > values_ )
            : data( data_ ), values( values_ )
         {}

         interval_set( std::initializer_list< std::pair< TypeT, TypeT > > common_ )
         {
            for( std::pair< TypeT, TypeT > p : common_ ) {
               if( p.first > p.second ) {
                  throw std::invalid_argument( "invalid interval" );
               }
               if( p.first == p.second ) {
                  continue;
               }
               if( !data.empty() && p.first < data.back() ) {
                  throw std::invalid_argument( "invalid interval" );
               }
               if( !data.empty() && p.first == data.back() ) {
                  data.pop_back();
                  values.pop_back();
                  data.push_back( p.second );
                  values.push_back( false );
                  continue;
               }
               data.push_back( p.first );
               data.push_back( p.second );
               values.push_back( true );
               values.push_back( false );
            }
         }

         explicit interval_set()
         {}

         ~interval_set() = default;
         interval_set( interval_set&& ) noexcept = default;
         interval_set& operator=( interval_set&& ) noexcept = default;
         interval_set( const interval_set& ) = delete;
         interval_set& operator=( const interval_set& ) = delete;

         void reserve( size_type n )
         {
            data.reserve( 2 * n );
         };
         size_type capacity() const noexcept
         {
            return data.capacity() / 2;
         };
         size_type size() const noexcept
         {
            return data.size() / 2;
         };

         bool empty() const noexcept
         {
            return data.empty();
         };

         AllocatorT get_allocator_type() const noexcept;  // returns the type of allocator.

         bool contains( const std::pair< TypeT, TypeT >& ) noexcept;

         std::pair< TypeT, TypeT > find( const TypeT ) noexcept;      // finds and returns specific interval.
         void clear() noexcept;                                       // Remove completely.
         void delete_interval( int pos ) noexcept;                    // used to remove posth interval. Can be used to delete first element. Left to right order.
         void delete_interval( std::pair< TypeT, TypeT > ) noexcept;  //delete specific interval.

         void append( const std::pair< TypeT, TypeT > ){};  // adds an interval to end of the list

         bool at( const TypeT ) noexcept
         {
            return false;
         };  // returns value at a spesific time

         interval_set< TypeT, AllocatorT >& set_union( const interval_set& other );
         interval_set< TypeT, AllocatorT >& set_difference( const interval_set& other );
         interval_set< TypeT, AllocatorT >& set_intersection( const interval_set& other );
         interval_set< TypeT, AllocatorT >& set_complement();

         interval_set< TypeT, AllocatorT >& operator+( const interval_set& other );
         interval_set< TypeT, AllocatorT >& operator-( const interval_set& other );
         interval_set< TypeT, AllocatorT >& operator&( const interval_set& other );
         interval_set< TypeT, AllocatorT >& operator not();

         // UTILITY
         std::string to_string() noexcept;
      };
   }  // namespace two_vecs
}  // namespace rvstd

#endif  // INTERVAL_SET_RVSTD