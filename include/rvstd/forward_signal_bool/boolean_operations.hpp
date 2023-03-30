#ifndef BOOLEAN_OPERATIONS_RVSTD
#define BOOLEAN_OPERATIONS_RVSTD

#include "forward_signal.hpp"
#include <iostream>

namespace rvstd
{
   namespace forward_signal_bool
   {
      inline unsigned int round_up_above_2_bit( unsigned int number )
      {
         number--;
         for( unsigned int i = 1; i < 16; i *= 2 ) {
            number |= number >> i;
         }
         number++;
         return number;
      }

      inline forward_signal set_operations( const forward_signal& first, const forward_signal& other, std::function< TypeV( const TypeV&, const TypeV& ) > op )
      {
         const TypeV* this_val_p = &first.get_init();
         const TypeV* other_val_p = &other.get_init();

         forward_signal ret( op( *this_val_p, *other_val_p ) );

         ret.reserve( round_up_above_2_bit( first.size() + other.size() ) );

         std::vector< std::pair< TypeT, TypeV > > ret_vec;
         int this_index = 0, other_index = 0;
         bool is_done_this = first.empty(), is_done_other = other.get_data_vector().empty();

         const TypeT* value = nullptr;
         while( !is_done_other || !is_done_this ) {
            if( is_done_other || ( !is_done_this && first.get_data_vector()[ this_index ].first <= other.get_data_vector()[ other_index ].first ) ) {
               this_val_p = &first.get_data_vector()[ this_index ].second;
               value = &first.get_data_vector()[ this_index ].first;
               ++this_index;
            }
            else {
               other_val_p = &other.get_data_vector()[ other_index ].second;
               value = &other.get_data_vector()[ other_index ].first;
               ++other_index;
            }

            ret.append( *value, op( *this_val_p, *other_val_p ) );

            is_done_this = this_index >= first.get_data_vector().size();
            is_done_other = other_index >= other.get_data_vector().size();
         }
         return ret;
      }

      inline forward_signal set_union( const forward_signal& first, const forward_signal& other )
      {
         std::function< TypeV( const TypeV&, const TypeV& ) > binary_op = []( const TypeV& x, const TypeV& y ) { return ( x || y ); };

         return set_operations( first, other, binary_op );
      }

      inline forward_signal set_intersection( const forward_signal& first, const forward_signal& other )
      {
         std::function< TypeV( const TypeV&, const TypeV& ) > binary_op = []( const TypeV& x, const TypeV& y ) { return ( x && y ); };
         return set_operations( first, other, binary_op );
      }

      inline forward_signal set_difference( const forward_signal& first, const forward_signal& other )
      {
         std::function< TypeV( const TypeV&, const TypeV& ) > binary_op = []( const TypeV& x, const TypeV& y ) { return ( x && !y ); };
         return set_operations( first, other, binary_op );
      }

      inline forward_signal set_complement( const forward_signal& first )
      {
         forward_signal not_this( !first.get_init() );
         //std::vector< std::pair< TypeT, TypeV > > new_data;

         TypeV temp;
         for( const std::pair< TypeT, TypeV >& p : first.get_data_vector() ) {
            temp = !p.second;
            not_this.append( p.first, temp );
            //new_data.push_back( p );
         }
         //not_this.set_data_vector( new_data );
         return not_this;
      }
   }  //namespace forward_signal_bool

}  // namespace rvstd

#endif  // BOOLEAN_OPERATIONS_RVSTD