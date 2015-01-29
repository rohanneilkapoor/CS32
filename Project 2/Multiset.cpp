#include <iostream>
#include "Multiset.h"

//constructor that initializes head to nullpointer and sets the count of the Multiset to 0
Multiset::Multiset()
{
    head = nullptr;
    counter = 0;
}

//copy constructor
Multiset::Multiset (const Multiset &src)
{
    head = nullptr; //sets head to nullpointer
    Multiset* res = new Multiset; //creates new multiset were all the src data will be placed
    //fills res with src data
    for(int i = 0; i < src.uniqueSize(); i++)
    {
        ItemType x;
        int a = src.get(i,x);
        for(int j = 0; j < a; j++)
        {
            res->insert(x);
        }
    }
    //swaps the current data with res and then deletes res
    this->swap(*res);
    delete res;
}

//assignment operator
Multiset& Multiset::operator= (const Multiset &src)
{
    //checks for aliasing
    if (&src == this)
    {
        return (*this);
    }
    //creates new multiset to fill with src data
    delete head;
    head = nullptr;
    counter = src.counter;
    Multiset *res = new Multiset;
    for(int i = 0; i < src.uniqueSize(); i++)
    {
        ItemType x;
        int a = src.get(i,x);
        for(int j = 0; j < a; j++)
        {
            res->insert(x);
        }
    }
    //swaps the current data with res and then deletes res
    this->swap(*res);
    delete res;
    return (*this);
}

//destructor
Multiset::~Multiset()
{
    //creates new node pointing to head
    Node* p = head;
    //iterates to all pointers in Multiset
    while (p != nullptr)
    {
        Node* x = p->next; //stores p->next in a temp variable
        delete p; // deletes p
        p = x; //iterates to next pointer
    }
}

//return true if the multiset is empty, otherwise false.
bool Multiset::empty() const
{
    //checks to see that both the head points to a nullptr and the count is 0
    if(head == nullptr && counter == 0)
    {
        return true;
    }
    return false;
}

// Return the number of items in the multiset.  For example, the size
// of a multiset containing "cumin", "cumin", "cumin", "turmeric" is 4.
int Multiset::size() const
{
    int count = 0; //counter variable
    Node* p = head;
    //iterates through entire Multiset
    while (p != nullptr)
    {
        //adds each individuals count to the local count variable
        count += p->value.count;
        p = p->next;
    }
    return count;
}

// Return the number of distinct items in the multiset.  For example,
// the uniqueSize of a multiset containing "cumin", "cumin", "cumin",
// "turmeric" is 2.
int Multiset::uniqueSize() const
{
    return counter; //return the counter variable
}

// Insert value into the multiset.  Return true if the value was
// actually inserted.  Return false if the value was not inserted
// (perhaps because the multiset has a fixed capacity and is full).
bool Multiset::insert(const ItemType& value)
{
    
    //adds if the Multiset is empty
    if (head == nullptr)
    {
        Node* p = new Node;
        p->value.element = value;
        p->value.count = 1;
        p->previous = p->next = nullptr;
        head = p;
        counter++;
        return true;
    }
    
    Node *pp = head;
    
    //adds 1 to the count if the element already exists
    while(pp != nullptr)
    {
        if(value == pp->value.element)
        {
            pp->value.count++;
            return true;
        }
        pp = pp->next;
    }
    

    //adds the new value if non existent but the Multiset has other elements
    
    //create new node
    Node* p = new Node;
    p->value.element = value;
    p->value.count = 1;

    
    //adds node to the front
    p->next = head;
    p->previous = nullptr;
    if(p->next != nullptr)
    {
        Node* x = p->next;
        x->previous = p;
    }
    head = p;
    counter++;
    return true;
}

// Remove one instance of value from the multiset if present.
// Return the number of instances removed, which will be 1 or 0.
int Multiset::erase(const ItemType& value)
{
    //checks if list is empty
    if(head == nullptr)
    {
        return 0;
    }
    
    //erases instance of first element
    if(head->value.element == value && head->value.count == 1)
    {
        Node* dest = head;
        head = dest->next;
        //if there is more than 1 value in the multiset, set the head's previous pointer to null
        if(counter > 1)
        {
            head->previous = nullptr;
        }
        delete dest;
        counter--;
        return 1;
    }
    
    //reduces the count of the element if there is more than one instance of that element
    else if (head->value.element == value && head->value.count > 1)
    {
        head->value.count--;
        return 1;
    }
    
    //erases instance of any other element
    Node *p = head;
    while (p != nullptr)
    {
        //sets p to the value before the value that needs to be deleted
        if(p->next != nullptr && p->next->value.element == value)
        {
            break;
        }
        p = p->next;
    }
    
    //deletes that value if it is the last of its kind
    if(p != nullptr && p->next->value.count == 1)
    {
        Node* dest = p->next;
        p->next = dest->next;
        //if not last item in list, set the previous pointer to the appropriate spot
        if(dest->next != nullptr)
        {
            Node* x = dest->next;
            x->previous = p;
        }
        delete dest;
        counter--;
        return 1;
    }
    
    //reduces the count of the element if there is more than one instance of that element
    else if(p != nullptr && p->next->value.count > 1)
    {
        p->next->value.count--;
        return 1;
    }
    return 0;
}

// Remove all instances of value from the multiset if present.
// Return the number of instances removed.
int Multiset::eraseAll(const ItemType& value)
{
    Node* p = head;
    //iterates through the Multiset to find the matched value
    while(p != nullptr)
    {
        if(p->value.element == value)
        {
            int count = p->value.count;
            //iterates through all of these same values
            for(int i = 0; i < count; i++)
            {
                //calls the erase fuction to delete each individual value that matches the parameter
                erase(p->value.element);
            }
            return count;
        }
        p = p->next;
    }
    return 0;
}

// Return true if the value is in the multiset, otherwise false.
bool Multiset::contains(const ItemType& value) const
{
    Node *p = head;
    //iterates through the entire Multiset
    while (p != nullptr)
    {
        //checks to see if an element matches that passed as a parameter
        if(value == p->value.element)
        {
            return true;
        }
        p = p->next;
    }
    return false;
}

// Return the number of instances of value in the multiset.
int Multiset::count(const ItemType& value) const
{
    Node *p = head;
    //iterates through the entire Multiset
    while (p != nullptr)
    {
        //returns the count of the individual element if it exists in the Multiset
        if(value == p->value.element)
        {
            return p->value.count;
        }
        p = p->next;
    }
    return 0;
}

// If 0 <= i < uniqueSize(), copy into value an item in the
// multiset and return the number of instances of that item in
// the multiset.  Otherwise, leave value unchanged and return 0.
// (See below for details about this function.)
int Multiset::get(int i, ItemType& value) const
{
    //checks to see if i is valid
    if(0 <= i && i < uniqueSize())
    {
        int count = 0;
        Node* p = head;
        //iterates through the entire Multiset
        while (p != nullptr)
        {
            //if the variable has been found, set the value equal to the element it matches and return the count of that element
            if(count == i)
            {
                value = p->value.element;
                return p->value.count;
            }
            count++;
            p = p->next;
        }
    }
    return 0;
}

// Exchange the contents of this multiset with the other one.
void Multiset::swap(Multiset& other)
{
    Node* temp = head; //creates temporary head pointer
    head = other.head; //sets this head equal to other head
    other.head = temp; //sets other head equal to the temporary head
    
    int tempCount = counter; //creates temporary counter
    counter = other.counter; //sets counter equal to other counter
    other.counter = tempCount; //sets other counter equal to temporary counter
}

//combines the elements of ms1 and ms2 into a single Multiset result
void combine(const Multiset& ms1, const Multiset& ms2, Multiset& result)
{
    Multiset* res = new Multiset; //creates a new multiset
    
    //adds ms1 to result by iterating through the entire ms1 and calling the get function
    for(int i = 0; i < ms1.uniqueSize(); i++)
    {
        ItemType x;
        ms1.get(i,x);
        for(int j = 0; j < ms1.get(i,x); j++)
        {
            res->insert(x);
        }
    }
    
    //adds ms2 to result by iterating through the entire ms2 and calling the get funciton
    for(int i = 0; i < ms2.uniqueSize(); i++)
    {
        ItemType y;
        ms2.get(i,y);
        for(int j = 0; j < ms2.get(i,y); j++)
        {
            res->insert(y);
        }
    }
    //swaps the newly created res Multiset with result and then deletes res
    res->swap(result);
    delete res;
}

//adds all the ms1 elements to result and then subtracts the ms2 elements from result
void subtract(const Multiset& ms1, const Multiset& ms2, Multiset& result)
{
    Multiset* res = new Multiset;
    
    //adds ms1 to result by iterating through the entire ms1 and calling the get function
    for(int i = 0; i < ms1.uniqueSize(); i++)
    {
        ItemType x;
        ms1.get(i,x);
        for(int j = 0; j < ms1.get(i,x); j++)
        {
            res->insert(x);
        }
    }
    
    //erases ms2 to result by iterating through the entire ms2 and calling the get funciton
    for(int i = 0; i < ms2.uniqueSize(); i++)
    {
        ItemType y;
        ms2.get(i,y);
        for(int j = 0; j < ms2.get(i,y); j++)
        {
            res->erase(y);
        }
    }
    //swaps the newly created res Multiset with result and then deletes res
    res->swap(result);
    delete res;
}