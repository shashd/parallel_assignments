#ifndef lacpp_sorted_list_hpp
#define lacpp_sorted_list_hpp lacpp_sorted_list_hpp
#include <atomic>

/* a sorted list implementation by David Klaftenegger, 2015
 * please report bugs or suggest improvements to david.klaftenegger@it.uu.se
 */

class TATAS_Lock{
	// delcare and init atomic state
	std::atomic<bool> state {false};

	public:

		void lock(){
			while(true){
				while(state.load()){}
				// try to acquire it
				if(!state.exchange(true)){
					return;
				}
			}
		}

		void unlock(){
			state = false;
		}

		void check_lock_free(){
			std::cout << (state.is_lock_free() ? "lock-free indeed :)" : "not lock-free") << std::endl;
		}
};

/* struct for list nodes */
template<typename T>
struct node {
	T value;
	node<T>* next;
	TATAS_Lock mutex;
};

/* non-concurrent sorted singly-linked list */
template<typename T>
class sorted_list {
	node<T> ground;	// just a sentinel
	node<T>* first = nullptr;

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
			node<T>* pred = &ground;
			// preparing the access to the first node
			pred->mutex.lock();

			/* first find position */
			node<T>* succ = first;
			while(succ != nullptr && succ->value < v) {
				succ->mutex.lock();
				pred->mutex.unlock();
				pred = succ;
				succ = succ->next;
			}
			/* construct new node */
			node<T>* current = new node<T>();
			current->value = v;

			/* insert new node between pred and succ */
			current->next = succ;
			if(pred == &ground) {
				first = current;
			} else {
				pred->next = current;
			}
            pred->mutex.unlock();
        }

		void remove(T v) {
			node<T>* pred = &ground;
			pred->mutex.lock();

            /* first find position */
			node<T>* current = first;
			while(current != nullptr && current->value < v) {
				current->mutex.lock();
				pred->mutex.unlock();
				pred = current;
				current = current->next;
			}
			if(current == nullptr || current->value != v) {
                pred->mutex.unlock();
                /* v not found */
				return;
			}
			/* remove current */
            current->mutex.lock();
			if(pred == &ground) {
				first = current->next;
			} else {
				pred->next = current->next;
			}
			pred->mutex.unlock();
			current->mutex.unlock();
			delete current;
        }

		/* count elements with value v in the list */
		std::size_t count(T v) {
			std::size_t cnt = 0;
			node<T>* past = &ground;
			ground.mutex.lock();
			
			/* first go to value v */
			node<T>* current = first;
			while(current != nullptr && current->value < v) {
				current->mutex.lock();
				past->mutex.unlock();
				past = current;
				current = current->next;
			}
			/* count elements */
			while(current != nullptr && current->value == v) {
				cnt++;

				current->mutex.lock();
				past->mutex.unlock();
				past = current;
				current = current->next;
			}

			past->mutex.unlock();
			return cnt;
		}
};

#endif // lacpp_sorted_list_hpp
