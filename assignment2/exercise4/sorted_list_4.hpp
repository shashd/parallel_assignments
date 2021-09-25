#ifndef lacpp_sorted_list_hpp
#define lacpp_sorted_list_hpp lacpp_sorted_list_hpp
#include <atomic>

/* a sorted list implementation by David Klaftenegger, 2015
 * please report bugs or suggest improvements to david.klaftenegger@it.uu.se
 */

class ttass_lock{
	// delcare and init atomic state
	// locked when true
	std::atomic<bool> state{false};

	public:
		void lock(){
			while(true){
				while(state.load()){}
				// return the old value of state
				if(!state.exchange(true)){
					return;
				}
			}
		}

		void unlock(){
			state = false;
		}
};
/* struct for list nodes */
template<typename T>
struct node {
	T value;
	ttass_lock lock;
	node<T>* next;
};

/* non-concurrent sorted singly-linked list */
template<typename T>
class sorted_list {

	node<T>* first = nullptr;
	ttass_lock head;
	ttass_lock tail;



	public:
		/* default implementations:
		 * default constructor
		 * copy constructor (note: shallow copy)
		 * move constructor
		 * copy assignment operator (note: shallow copy)
		 * move assignment operator
		 *
		 * The first is required due to the others,
		 * which are explicitly listed due to the rule of five.
		 */
		sorted_list() = default;
		sorted_list(const sorted_list<T>& other) = default;
		sorted_list(sorted_list<T>&& other) = default;
		sorted_list<T>& operator=(const sorted_list<T>& other) = default;
		sorted_list<T>& operator=(sorted_list<T>&& other) = default;
		~sorted_list() {
			while(first != nullptr) {
				remove(first->value);
			}
		}


		/* insert v into the list */
		void insert(T v) {

			/* construct new node
			* to ensure the node is initialized
			* so that it can perform lock and unlock functions
			*/
			node<T>* current = new node<T>();
			current->value = v;
			// lock the head first
			head.lock();
			if (first == nullptr){
				first = current;
				head.unlock();
				return ;
			}
			/* first find position */
			first->lock.lock();
			node<T>* pred = nullptr;
			node<T>* succ = first;
			// lock two nodes and hand over hand
			while(succ != nullptr && succ->value < v) {
				// unlock pred
				if (pred == nullptr){
					head.unlock();
				} else{
					pred->lock.unlock();
				}
				pred = succ;
				succ = succ->next;
				// lock new succ
				if (succ == nullptr){
					tail.lock();
				} else{
					succ->lock.lock();
				}
			}

			/* insert new node between pred and succ */
			current->next = succ;
			// if insert T.value < first.value
			if(pred == nullptr) {
				first = current;
				head.unlock();
			} else {
				pred->next = current;
				pred->lock.unlock();
			}
			// unlock succ
			if (succ == nullptr){
				tail.unlock();
			} else{
				succ->lock.unlock();
			}
		}

		void remove(T v) {
			/* first find position */
			head.lock();
			// if remove T v from an empty list
			if (first == nullptr){
				head.unlock();
				return ;
			}
			// lock first
			first->lock.lock();
			node<T>* pred = nullptr;
			node<T>* current = first;
			while(current != nullptr && current->value < v) {
				// unlock pred
				if (pred == nullptr){
					head.unlock();
				} else{
					pred->lock.unlock();
				}
				pred = current;
				current = current->next;
				// lock new current
				if (current == nullptr){
					tail.lock();
				} else{
					current->lock.lock();
				}
			}

			if(current == nullptr || current->value != v) {
				// unlock pred
				if (pred == nullptr){
					head.unlock();
				} else{
					pred->lock.unlock();
				}
				// unlock current
				if (current == nullptr){
					tail.unlock();
				} else{
					current->lock.unlock();
				}
				/* v not found */
				return;
			}
			/* remove current */
			if(pred == nullptr) {
				first = current->next;
				head.unlock();
			} else {
				pred->next = current->next;
				pred->lock.unlock();
			}
			current->lock.unlock();
			delete current;
		}
		/* count elements with value v in the list */
		std::size_t count(T v) {
			std::size_t cnt = 0;

			/* first go to value v */
			node<T>* current = first;
			while(current != nullptr && current->value < v) {
				current = current->next;
			}
			/* count elements */
			while(current != nullptr && current->value == v) {
				cnt++;
				current = current->next;
			}

			return cnt;
		}
};

#endif // lacpp_sorted_list_hpp
