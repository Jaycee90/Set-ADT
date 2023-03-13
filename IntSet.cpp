/**********************************************************************
Programmer: Jayce Turambe

 INVARIANT for the IntSet class:
 (1) Distinct int values of the IntSet are stored in a 1-D,
     compile-time array whose size is IntSet::MAX_SIZE;
     the member variable data references the array.
 (2) The distinct int value with earliest membership is stored
     in data[0], the distinct int value with the 2nd-earliest
     membership is stored in data[1], and so on.
     Note: No "prior membership" information is tracked; i.e.,
           if an int value that was previously a member (but its
           earlier membership ended due to removal) becomes a
           member again, the timing of its membership (relative
           to other existing members) is the same as if that int
           value was never a member before.
     Note: Re-introduction of an int value that is already an
           existing member (such as through the add operation)
           has no effect on the "membership timing" of that int
           value.
 (4) The # of distinct int values the IntSet currently contains
     is stored in the member variable used.
 (5) Except when the IntSet is empty (used == 0), ALL elements
     of data from data[0] until data[used - 1] contain relevant
     distinct int values; i.e., all relevant distinct int values
     appear together (no "holes" among them) starting from the
     beginning of the data array.
 (6) We DON'T care what is stored in any of the array elements
     from data[used] through data[IntSet::MAX_SIZE - 1].
     Note: This applies also when the IntSet is empry (used == 0)
           in which case we DON'T care what is stored in any of
           the data array elements.
     Note: A distinct int value in the IntSet can be any of the
           values an int can represent (from the most negative
           through 0 to the most positive), so there is no
           particular int value that can be used to indicate an
           irrelevant value. But there's no need for such an
           "indicator value" since all relevant distinct int
           values appear together starting from the beginning of
           the data array and used (if properly initialized and
           maintained) should tell which elements of the data
           array are actually relevant.
**************************************************************************/

#include "IntSet.h"
#include <iostream>
#include <cassert>
using namespace std;

//Constructor initialized to an empty IntSet
IntSet::IntSet() : used(0){}

int IntSet::size() const
{
   //return total element in the array.
   return used;
}

bool IntSet::isEmpty() const
{
   //empty is true if IntSet has no relevant elements.
   return this->size() == 0;
}

bool IntSet::contains(int anInt) const
{	
	//traverse IntSet looking for an anInt
	//return false if anInt is not found.  
	if (used > 0){
		for(int i = 0; i < used; i++)
			if (data[i] == anInt) return true;
	}
	return false;
}

bool IntSet::isSubsetOf(const IntSet& otherIntSet) const
{
	//check if IntSet is empty and return true since
   //empty set is always a subset of another set.
   if (this->isEmpty()){
   	return true;
   }else{
   	//loop through and check if all elements of the invoking IntSet
    //are also elements of otherIntSet, otherwise return false.
    for(int i =0; i < this->size(); i++)
    	if (!otherIntSet.contains(this->data[i])) return false;
    		
	return true;
   }
}

void IntSet::DumpData(ostream& out) const
{
   if (used > 0)
   {
      out << data[0];
      for (int i = 1; i < used; ++i)
         out << "  " << data[i];
   }
}

IntSet IntSet::unionWith(const IntSet& otherIntSet) const
{
	//The union of the invoking set and otherIntSet
	//should not exceed the MAX_SIZE.
	assert(size() + (otherIntSet.subtract(*this)).size() <= MAX_SIZE);
	
	//make a copy of the invoking set
	IntSet myUnionset = *this;
	
	//loop through otherIntSet to find the elements that  
	//are not in the invoking set, if found addd them
	for (int i = 0; i < otherIntSet.size(); i++){
		if (!myUnionset.contains(otherIntSet.data[i])){
   			myUnionset.add(otherIntSet.data[i]);
	   }
   }
	return myUnionset; 
}

IntSet IntSet::intersect(const IntSet& otherIntSet) const
{
   //A copy of the invoking IntSet
   IntSet myIntersect = *this;

    //loop through to find elements of the invoking set that 
   //are not in the otherIntSet. If found, remove them
   for (int i = 0; i < size(); i++){
   	if (!otherIntSet.contains(data[i])){
   		myIntersect.remove(data[i]);
	   }
   }
   return myIntersect; 
}

IntSet IntSet::subtract(const IntSet& otherIntSet) const
{
	//Make a copy of the invoking IntSet
	IntSet mySubset = *this;
	
	//subtract otherIntSet from the invoking set
	for (int i = 0; i < otherIntSet.size(); i++){
		if (mySubset.contains(otherIntSet.data[i])){
			mySubset.remove(otherIntSet.data[i]);
		}
	}
	return mySubset;
}

void IntSet::reset()
{
	//empty the invoking IntSet
	used = 0;
}

bool IntSet::add(int anInt)
{
	//perform error check for duplicates and check if the 
	//number in the invoking set is less than the MAX_SIZE
   assert((contains(anInt)) ? size() <= MAX_SIZE : size() < MAX_SIZE);

   if (!contains(anInt)){
   		data[used] = anInt;
   		used++;
   		return true;
   }
   //the invoking IntSet is unchanged
   return false; 
}

bool IntSet::remove(int anInt)
{
	//Shifting elements of array data with used items 
	//when removing a anInt-matching item
	if (contains(anInt)){
		for (int i = 0; i < used; i++){
			if (data[i] == anInt){
				for (int j = i + 1; j < used; j++){
					data[j-1] = data[j];
				}
				used--;
				return true;
			}
		}
	}
	//the invoking IntSet is unchanged
	return false;
}

bool equal(const IntSet& is1, const IntSet& is2)
{
	//Check if is1 is a subset of is2 and is2 is a subset of is1.
	//If true, then they are equal by the defintion of subset
	if (is1.IntSet::isSubsetOf(is2) && is2.IntSet::isSubsetOf(is1))
		return true;
	
	//return false otherwise
   return false;
}
