/**
 * Buffer circulaire générique pour des caractères ( nSize doit-être puissance de 2 )
 * @author     Marc Juneau (marcjuneau@gmail.com)
 * @version    0.01
 * @date       4 juin 2015
 */
#ifndef BUFFER_H_
#define BUFFER_H_

#include <stdint-gcc.h>

template<class T, uint16_t nSize>
class Buffer {
public:
	/**
	 * @brief Constructeur
	 */
	Buffer() {
		reset();
	}
	/**
	 * @brief Remise à zéro des index
	 */
	void reset() {
		in = 0;
		out = 0;
		count = 0;
		data[nSize-1] = 0;
	}
	/**
	 * @brief Ajout d'un element
	 * @param c un élément de type T
	 */
	bool add(T e) {
		if ((in + 1) != out) {
			data[in++ & (nSize - 1)] = e;
			count++;
			return 1;
		}
		return 0;
	}
	/**
	 * @brief Retire un élément
	 * @return L'élément retiré, ou 0 si le buffer était vide.
	 */
	T rem() {
		if (!isEmpty()) {
			count--;
			return data[out++ & (nSize - 1)];
		}
		return 0;
	}
	/**
	 * @brief Dit si le buffer est vide.
	 * @return 1 si le buffer est vide
	 */
	bool isEmpty() {
		return (out == in);
	}

	uint16_t getCount() const {
		return count;
	}

private:
	uint16_t in;
	uint16_t out;
	uint16_t count;
	T data[nSize];
};

#endif /* BUFFER_H_ */
