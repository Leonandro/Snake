#ifndef LINKED_LIST
#define LINKED_LIST

#include <cstdlib> // size_t
#include <ostream>
#include <initializer_list>
namespace sc
{

    /** 
    \brief That is a template class the implements a doubly linked list
    */
    template <typename T>
    class list
    {
        private:
        struct node
        {
            T data;
            node* next;
            node* prev;

            bool operator==(node & rhs){
                return data==rhs.data & next == rhs.next && rhs.prev == prev;
            }
        };

        public:
        class iterator
            {
                public:
                    typedef std::ptrdiff_t difference_type;
                    typedef std::bidirectional_iterator_tag iterator_category;
                    typedef T& reference; //!< Alias para uma referência.
                    typedef node * pointer;
                    typedef node value_type;

                    //=== Private data
                private:
                    node* ptr;
                public:
                    iterator( node * pt = nullptr ) : ptr{ pt }
                    { /* empty */ }
                    /// it1 = it2;
                    iterator(const iterator & other){
                        this->ptr = other.ptr;
                    }
                    iterator& operator=( const iterator& rhs )
                    {
                        ptr = rhs.ptr;
                        return *this;
                    }
                    /// *it = x;
                    reference operator*(void)
                    {
                        return ptr->data;
                    }
                    /// x = *it;
                    const reference operator*(void) const
                    {
                        return ptr->data;
                    }
                    /// ++it pré incremento.
                    iterator operator++(void)
                    {
                        //++ptr;
                        //return *this;
                        // Retorna um novo iterador que aponta para o próximo endereço.

                        ptr = ptr->next;

                        return iterator(ptr);
                    }
                    /// it++ pós incremento.
                    iterator operator++(int)
                    {
                        ptr = ptr->next;
                        return ptr->prev ;
                    }
                    /// --it pré decremento.
                    iterator operator--(void)
                    {
                        ptr = ptr->prev;
                        return iterator ( ptr->next );
                    }
                    /// it-- pós decremento.
                    iterator operator--(int)
                    {
                        // Opção 1:
                        iterator temp( ptr ); // Cria um iterator temporário com o endereço atual.
                        ptr = ptr->prev;
                        return temp;
                    }
                    pointer& getpointer(){
                        return ptr;
                    }
                    
                    /// Comparação it == it2
                    bool operator==( const iterator &it ) const
                    { 
                        //std::cout <<"ENDEREÇO DO THIS: "<< this->ptr<<std::endl;
                        //std::cout <<"ENDEREÇO DO PARAMETRO : "<<it.ptr << std::endl;
                        return this->ptr == it.ptr ; /* Devem apontar pro mesmo endereço. */ 
                    }
                    /// Comparação it != it2
                    bool operator!=( const iterator &it ) const
                    { 
                        bool equal = *this == it;
                        //std::cout << equal << std::endl;
                        //std::cout << !equal <<std::endl;
                        return  !equal ; /* Não Devem apontar pro mesmo endereço. */ 
                    }
                    /// it = it1 - it2;
                    
                    iterator operator+( int a  )
                    {
                        iterator aux (ptr);
                        int i = 0;
                        while (i < a){
                            aux++;
                            i++;
                        }
                        return aux;
                    }
                    iterator operator-( int a  )
                    {
                        iterator aux (ptr);
                        int i = 0;
                        while (i < a){
                            aux--;
                            i++;
                        }
                        return aux;
                    }
            };
                
        private:
        node* tail;
        size_t SIZE;
        node* head;
        public:
        /** 
         \brief Regular constructor
         
        */
        list()
        {
            head = new node();
            tail = new node();
            head->next = tail;
            tail->prev = head;
            head->prev = tail->next = nullptr;
            SIZE = 0;
        }

        /** 
         \brief Copy constructor
         @param List to be copied
        */
        list( const list &other){
            head = new node();
            tail = new node();
            head->next = tail;
            tail->prev = head;
            head->prev = tail->next = nullptr;
            SIZE = 0;
            node* it = other.tail->prev;
            
            while (it != other.head){
                this->push_front(it->data);
                it = it->prev;
            }
        }

        /** 
         \brief Using initializer list constructor
         @param ilist : initializer list with the values for the list
        */
        list(const std::initializer_list<T>& ilist)
        {
            head = new node();
            tail = new node();
            head->next = tail;
            tail->prev = head;
            SIZE = 0;
            head->prev = tail->next = nullptr;
            auto it = ilist.begin();
            for (; it != ilist.end() ; it++){
                push_back( *it );
            }
        }
        /** 
         \brief With a initial size constructor
         @param sz: size of the new list
        */
        list(size_t sz)
        {
            head = new node();
            tail = new node();
            head->next = tail;
            tail->prev = head;
            head->prev = tail->next = nullptr;
            T e;
            for (int i =0; i< sz; i++){
                push_back(e);
            }
            SIZE = sz;
        }
        /** 
         \brief Range constructor
         @param first: operator to the begin of the range
         @param last : operator to the end of the range
        */
        template<typename InItr >
        list(InItr first, InItr last)
        {
            head = new node();
            tail = new node();
            head->next = tail;
            tail->prev = head;
            head->prev = tail->next = nullptr;
            SIZE = 0;
            while (first != last){
                push_back(*first);
                first++;
            }

        }
        /** 
         \brief Destructor
        */
        ~list(){
            clear();
            delete head;
            delete tail;
        }

        /** 
         \brief Add a element to at the first position of the list
         @param data: value to be added
        */
        void push_front( const T& data)
        {
            node* target = new node();
            target->data = data;
            target->prev = head;
            target->next = head->next;
            head->next->prev = target;
            head->next = target;
            SIZE++;
        }
        /** 
         \brief Add a element to the last position of the list
         @param data: value to be added
        */
        void push_back( const T& data)
        {
            node* target = new node();
            target->data = data;
            target->next = tail;
            target->prev = tail->prev;
            tail->prev->next = target;
            tail->prev = target;
            SIZE++;
        }
        /** 
         \brief Clear the list
        */
        void clear(){
            if (head->next == tail) return;
            while (head->next != tail){
                pop_front();
            }
            SIZE = 0;
        }

        /** 
         \brief Returns the current size of the list
        */
        size_t size()
        {
            return SIZE;
        }
        /** 
         \brief Return if the list is empty
        */
        bool empty(){
            return  head->next == tail;
        }
        /** 
         \brief Remove the element at the top of the list
        */
        void pop_front()
        {
            if (head->next == tail) return;
            node* target = head->next;
            head->next->next->prev = head;
            head->next = head->next->next;
            delete target;
            SIZE--;    
        }

        /** 
         \brief Remove the element at the last position of the list
        */
        void pop_back()
        {
            if (head->next == tail) return;
            node* target = tail->prev;
            tail->prev->prev->next = tail;
            tail->prev = tail->prev->prev;
            delete target;
            SIZE--;
        }
        /** 
         \brief Insert the value at the a given position
         @param pos: Iterator to the position of the insertion
         @param value: Value the is about to be insert
        */
        void insert(iterator pos, const T & value)
        {
            node* target = new node();
            target->data = value;
            
            target->prev = pos.getpointer()->prev;
            target->next = pos.getpointer();
            pos.getpointer()->prev = target;
            target->prev->next = target; 
            
            SIZE++;
        }
        /** 
         \brief Replace the values of the list for 'value'
         @param value: New value of the positions of the list
        */
        void assing(const T& value)
        {
            iterator it = begin();
            while (it != tail)
                *it++ = value;
        }
        /** 
         \brief Replace the list for count nodes with value 
         @param count: New size of the list
         @param value: New value for the list nodes 
        */
        void assing(const T& value, size_t count){
            clear();
            for (int i =0; i< count; i++)
                push_front(value);

        }
        /** 
         \brief Insert at a given position the values in the given range
         @param pos: Iterator to the position of the insertion
         @param first: Iterator to the begin of the range
         @param last: Iterator to the end of the range
        */
        template<typename InItr >
        iterator insert( iterator pos, InItr first, InItr  last )
        {  
            iterator aux = pos.getpointer()->prev; 
            while (first != last)
            {
                insert(pos, *first);
                first++;
            }
            return aux;
        }

        /** 
         \brief Insert the values of a iniliazed list at a given position
         @param pos: Iterator to the position of the insertion
         @param ilist: Initializer list with the values to be inserted
        */
        iterator insert(iterator pos, std::initializer_list<T> ilist)
        {
            iterator aux = pos.getpointer()->prev; 
            auto it =  ilist.begin();
            while (it != ilist.end()){
                insert(pos, *it++);
            }
            return aux;
        }
        /** 
         \brief Remove from the list the value at the given position
         @param pos: Iterator to the postion to be removed
        */
        iterator erase(iterator pos)
        {
            node* to_delete = pos.getpointer();
            iterator aux = pos+1;
            
            to_delete->prev->next = to_delete->next;
            to_delete->next->prev =  to_delete->prev;
            delete to_delete;
            SIZE--;
            return aux;
        }
        /** 
         \brief Remove from the list the values of the given range
         @param first: Iterator to the begin of the range
         @param last:  Iterator to the end of the range
        */
        iterator erase( iterator first, iterator last)
        {
            iterator pos = first;
            while (first != last){
                erase(++first - 1 );
            }

            return last;
        }
        /** 
         \brief Return a const reference for the last value of the list
        */
        const T& back() const{
            if (head->next == tail) 
                throw "[]";
            return tail->prev->data;
        }

        T& back(){
            if (head->next == tail) 
                throw "[]";
            return tail->prev->data;
        }

        /** 
         \brief Return a const referece to the firs value of the list
        */
        const T& front() const{
            if (head->next == tail) 
                throw "[]";
            return head->next->data;
        }
        T& front() {
            if (head->next == tail) 
                throw "[]";
            return head->next->data;
        }



        /** 
         \brief Return a iterator to the firs position of the list
        */
        iterator begin (){
            return iterator(head->next);
        }
        /** 
         \brief Return a iterator to the last value of the list
        */
        iterator end(){
            return iterator(tail);
        }
        /** 
         \brief Operator of atribution
         @param rhs: The list the 'this' is now equal to
        */
        list& operator=(list & rhs)
        {
            clear();
            node* it = rhs.head->next;
            while (it != rhs.tail){
                push_back(it->data);
                it = it->next;
            }
            return *this;

        }
        /** 
         \brief Operator of atribution the recive a initilazer list
         @param ilist: Initializer list with the values for 'this'
        */
        list& operator=(const std::initializer_list<T> ilist){
            clear();
            auto it = ilist.begin();
            while (it != ilist.end()){
                push_back(*it++);
            }
            return *this;
        }

        /** 
         \brief operator of comparation (if the lists are equal)
         @param rhs: The list to be compared with 'this'
        */
        bool operator==(const list &  rhs){
            if(rhs.SIZE != SIZE) return false;
            
            iterator it (head->next);
            iterator it2( rhs.head->next) ;
            while(it != tail)
            {
                if (*it != *it2)
                {
                    return false;
                }
                it ++;
                it2 ++;
            }
            return true;
        }
        /** 
         \brief operator of comparation (if the lists are diferent)
         @param rhs: The list to be compared with 'this'
        */
        bool operator!=(const list & rhs){
            return not (*this == rhs);
        }

        /** 
         \brief operator of extraction
        */
        friend std::ostream& operator<<(std::ostream & os, const list &list){
            node* it;
            it = list.head->next;
            os<< "[ ";
            while(it != list.tail){
                os << it->data << " ";
                it = it->next;
            }
            os<<"]";
            return os;
        }

    };

};

#endif