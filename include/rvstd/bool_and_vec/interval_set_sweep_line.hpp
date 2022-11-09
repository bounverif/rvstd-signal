#ifndef INTERVAL_SET_RVSTD
#define INTERVAL_SET_RVSTD

#include <algorithm>
#include <chrono>
#include <climits>
#include <cstdlib>
#include <new>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>

namespace rvstd
{
   namespace bool_and_vec_sweep_line
   {
      template< class TypeT = double, class AllocatorT = std::allocator< TypeT > >
      class interval_set
      {
      private:
         std::vector< TypeT > data;  // Data buffer
         bool init;

      public:
         using size_type = std::size_t;

         interval_set( std::initializer_list< std::pair< TypeT, TypeT > > common_, bool init_ = false )
         {
            init = init_;
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
                  data.push_back( p.second );
                  continue;
               }
               data.push_back( p.first );
               data.push_back( p.second );
            }
         }

         explicit interval_set( bool init_ = false )
            : init( init_ ){};

         ~interval_set() = default;
         interval_set( interval_set&& ) noexcept = default;
         interval_set& operator=( interval_set&& ) noexcept = default;
         interval_set( const interval_set& ) = default;
         interval_set& operator=( const interval_set& ) = default;

         void reserve( size_type n )
         {
            data.reserve( n );
         };
         size_type capacity() const noexcept
         {
            return data.capacity();
         };
         size_type size() const noexcept
         {
            return data.size();
         };

         bool empty() const noexcept
         {
            return data.empty();
         };

         std::string get_allocator_type() const noexcept
         {
            return ( std::string )( typeid( AllocatorT ).name );
         };  // returns the type of allocator.

         bool contains( const std::pair< TypeT, TypeT >& ) noexcept;

         std::pair< TypeT, TypeT > find( const TypeT ) noexcept;      // finds and returns specific interval.
         void clear() noexcept;                                       // Remove completely.
         void delete_interval( int pos ) noexcept;                    // used to remove posth interval. Can be used to delete first element. Left to right order.
         void delete_interval( std::pair< TypeT, TypeT > ) noexcept;  //delete specific interval.

         /*
         * adds an element to end of the list
         * O(1)
         */
         void append( TypeT pos, bool value )
         {
            if( data.size() == 0 && value == init ) {
               return;
            }
            if( data.size() == 0 && value != init ) {
               data.push_back( pos );
               return;
            }

            if( pos < data.back() ) {
               throw( " cannot add to mid of the list " );
            }

            if( value == end_value() ) {
               return;
            }

            if( pos == data.back() ) {
               data.pop_back();
               return;
            }
            data.push_back( pos );
            return;
         };

         /*
        * O(1)
        * !A != !B => XOR operation with normalizing bools
        */
         bool end_value()
         {
            return ( !init != !( data.size() % 2 ) );
         }

         /*
         * O(log(n))
         * returns value at a spesific time
         */
         bool at( const TypeT pos )
         {
            if( data.size() == 0 ) {
               return init;
            }
            typename std::vector< TypeT >::iterator up;
            up = std::upper_bound( data.begin(), data.end(), pos );
            int index = up - data.begin();  //how much change
            return ( !init != !( index % 2 ) );
         };

         template< typename TypeP >
         interval_set< TypeT, AllocatorT > set_operations( const interval_set< TypeT, AllocatorT >& other, std::function< TypeP( TypeP, TypeP ) > op )
         {
            bool this_bool = init;
            bool other_bool = other.get_init();

            //assign init of list constructed
            bool ret_bool = op( this_bool, other_bool );

            //create new list
            interval_set< TypeT, AllocatorT > ret( ret_bool );

            ret.reserve( round_up_above_2_bit( data.size() + other.size() ) );

            std::vector< TypeT > other_vec = other.get_data_vector();
            std::vector< TypeT > ret_vec;
            int this_index = 0, other_index = 0;
            bool is_done_this = ( data.size() == 0 ), is_done_other = ( other_vec.size() == 0 );

            while( !is_done_other || !is_done_this ) {
               TypeT value;

               if( is_done_other || ( !is_done_this && data[ this_index ] <= other_vec[ other_index ] ) ) {
                  this_bool = !this_bool;
                  value = data[ this_index ];
                  ++this_index;
               }
               else if( is_done_this || ( !is_done_other && data[ this_index ] > other_vec[ other_index ] ) ) {
                  other_bool = !other_bool;
                  value = other_vec[ other_index ];
                  ++other_index;
               }
               bool calc_bool = op( this_bool, other_bool );
               ret.append( value, calc_bool );

               is_done_this = this_index >= data.size();
               is_done_other = other_index >= other_vec.size();
            }
            return ret;
         }

         interval_set< TypeT, AllocatorT > set_union( const interval_set< TypeT, AllocatorT >& other )
         {
            std::function< bool( bool, bool ) > binary_op = []( bool x, bool y ) { return ( x || y ); };
            return set_operations< bool >( other, binary_op );
         }

         interval_set< TypeT, AllocatorT > set_difference( const interval_set< TypeT, AllocatorT >& other )
         {
            std::function< bool( bool, bool ) > binary_op = []( bool x, bool y ) { return ( x && !y ); };
            return set_operations< bool >( other, binary_op );
         }
         interval_set< TypeT, AllocatorT > set_intersection( const interval_set< TypeT, AllocatorT >& other )
         {
            std::function< bool( bool, bool ) > binary_op = []( bool x, bool y ) { return ( x && y ); };
            return set_operations< bool >( other, binary_op );
         }
         interval_set< TypeT, AllocatorT > set_complement()
         {
            interval_set< TypeT, AllocatorT > not_this( !init );
            not_this.set_data_vector( data );
            return not_this;
         }

         interval_set< TypeT, AllocatorT > operator+( const interval_set< TypeT, AllocatorT >& other )
         {
            return set_union( other );
         }
         interval_set< TypeT, AllocatorT > operator-( const interval_set< TypeT, AllocatorT >& other )
         {
            return set_difference( other );
         }
         interval_set< TypeT, AllocatorT > operator&( const interval_set< TypeT, AllocatorT >& other )
         {
            return set_intersection( other );
         }
         interval_set< TypeT, AllocatorT > operator not()
         {
            return set_complement();
         }

         // UTILITY
         const std::string to_string();
         const std::vector< TypeT >& get_data_vector() const
         {
            return data;
         }
         bool get_init() const
         {
            return init;
         }
         void set_data_vector( const std::vector< TypeT > data_ )
         {
            data = data_;
         }
         unsigned int round_up_above_2_bit( unsigned int x )
         {
            x--;
            for( unsigned int i = 1; i < 16; i *= 2 ) {
               x |= x >> i;
            }
            x++;
            return x;
         }
      };
   }  // namespace bool_and_vec_sweep_line
}  // namespace rvstd

#endif  // INTERVAL_SET_RVSTD
