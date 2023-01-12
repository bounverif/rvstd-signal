#ifndef INTERVAL_SET_RVSTD
#define INTERVAL_SET_RVSTD

#include <algorithm>
#include <climits>
#include <cstdlib>
#include <new>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>

namespace rvstd
{
   namespace vec_of_pairs
   {
      template< class TypeT = double, class AllocatorT = std::allocator< TypeT > >
      class interval_set
      {
      private:
         std::vector< std::pair< TypeT, int > > data;  // Data buffer
         int base = 1;
         int init;

      public:
         using size_type = std::size_t;

         interval_set( std::initializer_list< std::pair< TypeT, TypeT > > common_, int init_ = 0 )
         {
            init = init_;

            for( std::pair< TypeT, TypeT > p : common_ ) {
               if( p.first > p.second ) {
                  throw std::invalid_argument( "invalid interval" );
               }
               if( p.first == p.second ) {
                  continue;
               }
               if( !data.empty() && p.first < data.back().first ) {
                  throw std::invalid_argument( "invalid interval" );
               }
               if( !data.empty() && p.first == data.back().first ) {
                  data.pop_back();
                  data.push_back( std::make_pair( p.second, init ) );
                  continue;
               }
               data.push_back( std::make_pair( p.first, !init ) );
               data.push_back( std::make_pair( p.second, init ) );
            }
         }

         explicit interval_set( int init_ = 0 )
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
         void append( TypeT pos, int value )
         {
            if( data.size() == 0 && value == init ) {
               return;
            }
            if( data.size() == 0 && value != init ) {
               data.push_back( std::make_pair( pos, value ) );
               return;
            }

            if( pos < data.back().first ) {
               throw( " cannot add to mid of the list " );
            }

            if( value == data.back().second ) {
               return;
            }

            if( pos == data.back().first ) {
               data.pop_back();
               return;
            }
            data.push_back( std::make_pair( pos, value ) );
         };

         /*
         * O(log(n))
         * returns value at a spesific time
         */
         int at( const TypeT pos )
         {
            if( data.size() == 0 ) {
               return init;
            }
            typename std::vector< std::pair< TypeT, int > >::iterator up;

            up = std::lower_bound( data.begin(), data.end(), std::make_pair( pos, 0 ), []( const std::pair< TypeT, int >& a, const std::pair< TypeT, int >& b ) { return a.first <= b.first; } );

            int index = std::distance( data.begin(), up ) - 1;
            return index < 0 ? init : data[ index ].second;
         };

         template< typename TypeP >
         interval_set< TypeT, AllocatorT > set_operations( const interval_set< TypeT, AllocatorT >& other, std::function< TypeP( TypeP, TypeP ) > op )
         {
            TypeP this_val = init;
            TypeP other_val = other.get_init();

            //assign init of list constructed
            TypeP ret_bool = op( this_val, other_val );

            //create new list
            interval_set< TypeT, AllocatorT > ret( ret_bool );

            ret.reserve( round_up_above_2_bit( data.size() + other.size() ) );

            std::vector< std::pair< TypeT, TypeP > > other_vec = other.get_data_vector();
            std::vector< std::pair< TypeT, TypeP > > ret_vec;
            int this_index = 0, other_index = 0;
            int is_done_this = ( data.size() == 0 ), is_done_other = ( other_vec.size() == 0 );

            while( !is_done_other || !is_done_this ) {
               TypeT value;

               if( is_done_other || ( !is_done_this && data[ this_index ].first <= other_vec[ other_index ].first ) ) {
                  this_val = data[ this_index ].second;
                  value = data[ this_index ].first;
                  ++this_index;
               }
               else if( is_done_this || ( !is_done_other && data[ this_index ].first > other_vec[ other_index ].first ) ) {
                  other_val = other_vec[ other_index ].second;
                  value = other_vec[ other_index ].first;
                  ++other_index;
               }
               TypeP calc_val = op( this_val, other_val );
               ret.append( value, calc_val );

               is_done_this = this_index >= data.size();
               is_done_other = other_index >= other_vec.size();
            }
            return ret;
         }

         //maxs
         interval_set< TypeT, AllocatorT > set_union( const interval_set< TypeT, AllocatorT >& other )
         {
            std::function< int( int, int ) > binary_op = []( int x, int y ) { return ( x < y ? y : x ); };
            return set_operations< int >( other, binary_op );
         }
         interval_set< TypeT, AllocatorT > set_difference( const interval_set< TypeT, AllocatorT >& other )
         {
            std::function< int( int, int ) > binary_op = []( int x, int y ) { return ( x > y ); };
            return set_operations< int >( other, binary_op );
         }
         interval_set< TypeT, AllocatorT > set_intersection( const interval_set< TypeT, AllocatorT >& other )
         {
            std::function< int( int, int ) > binary_op = []( int x, int y ) { return ( x < y ? x : y ); };
            return set_operations< int >( other, binary_op );
         }
         interval_set< TypeT, AllocatorT > set_complement()
         {
            interval_set< TypeT, AllocatorT > not_this( base - init );
            std::vector< std::pair< TypeT, int > > new_data;

            for( std::pair< TypeT, int > p : data ) {
               new_data.push_back( std::make_pair( p.first, base - p.second ) );
            }
            not_this.set_data_vector( new_data );
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
         const std::vector< std::pair< TypeT, int > >& get_data_vector() const
         {
            return data;
         }
         int get_init() const
         {
            return init;
         }
         void set_data_vector( const std::vector< std::pair< TypeT, int > >& data_ )
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
   }  // namespace vec_of_pairs
}  // namespace rvstd

#endif  // INTERVAL_SET_RVSTD