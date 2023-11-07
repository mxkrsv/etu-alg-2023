#include <assert.h>
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IS_A_POWER_OF_TWO(x) x != 0 && (x & (x - 1)) == 0

#ifdef DEBUG
#define DPRINTF(...) fprintf(stderr, __VA_ARGS__)
#else
#define DPRINTF(...) ;
#endif

template <typename T> class square_matrix {
	public:
	square_matrix(size_t size);
	square_matrix(char *);
	square_matrix(const square_matrix<T> &);
	~square_matrix();

	void add(const square_matrix<T> *);
	void sub(const square_matrix<T> *);
	void mul(T);
	void mul(const square_matrix<T> *);

	square_matrix<T> operator=(const square_matrix<T> &op) {
		*this = square_matrix<T>(op);
		return *this;
	}
	square_matrix<T> operator+(const square_matrix<T> op) {
		square_matrix<T> nm(this);
		nm.add(&op);
		return nm;
	};
	square_matrix<T> operator-(const square_matrix<T> op) {
		square_matrix<T> nm(this);
		nm.sub(&op);
		return nm;
	};
	square_matrix<T> operator*(T op) {
		DPRINTF("multiplication operator called\n");
		square_matrix<T> nm(this);
		nm.mul(&op);
		return nm;
	};
	square_matrix<T> operator*(const square_matrix<T> op) {
		square_matrix<T> nm(*this);
		nm.mul(&op);
		return nm;
	};

	square_matrix<T> *submatrix(const size_t[], const size_t[], size_t);

	char *to_string();
	size_t get_size();

	T **rows;

	protected:
	size_t size;

	private:
	void from_string(char *);
};

template <typename T> square_matrix<T>::square_matrix(size_t size) {
	assert(size > 0);

	DPRINTF("allocation constructor called\n");

	this->size = size;
	this->rows = (T **)malloc(sizeof(this->rows[0]) * size);
	assert(this->rows != NULL);

	for (size_t i = 0; i < size; i++) {
		this->rows[i] = (T *)malloc(sizeof(this->rows[0][0]) * size);
		assert(this->rows[i] != NULL);
	}
}

template <typename T> square_matrix<T>::square_matrix(char *s) {
	this->from_string(s);
}

template <typename T>
square_matrix<T>::square_matrix(const square_matrix<T> &other)
    : square_matrix(other.size) {
	assert(other.rows && other.size);
	assert(this->rows && this->size);

	DPRINTF("copy constructor called\n");

	for (size_t i = 0; i < this->size; i++) {
		assert(this->rows[i] && other.rows[i]);
		memcpy(this->rows[i], other.rows[i],
		       sizeof(this->rows[0][0]) * this->size);
	}
}

template <typename T> square_matrix<T>::~square_matrix() {
	assert(this);
	assert(this->size);
	assert(this->rows);

	for (size_t i = 0; i < this->size; i++) {
		assert(this->rows[i]);
		free(this->rows[i]);
	}

	free(this->rows);
}

template <typename T> char *square_matrix<T>::to_string() {
	assert(this);
	assert(this->rows);
	char *ret = (char *)malloc(sizeof(char) * 1024);
	assert(ret);

	char *pos = ret;
	for (size_t i = 0; i < this->size; i++) {
		assert(this->rows[i]);
		for (size_t j = 0; j < this->size; j++) {
			DPRINTF("%zu: %p\n", i, this->rows[i]);
			pos += sprintf(pos, "%d%c", this->rows[i][j],
				       j == this->size - 1 ? '\n' : ' ');
		}
	}

	return ret;
}

template <typename T> size_t square_matrix<T>::get_size() {
	return this->size;
}

static size_t count_tokens(const char *str, const char *sep) {
	assert(str != NULL && sep != NULL);

	char *tmp = (char *)malloc(sizeof(tmp[0]) * (strlen(str) + 1));
	assert(tmp != NULL);
	strcpy(tmp, str);

	size_t ret = 1;
	char *save = NULL;
	for (assert(strtok_r(tmp, sep, &save) != NULL);
	     strtok_r(NULL, sep, &save) != NULL; ret++) {
	}

	free(tmp);
	return ret;
}

template <typename T> static T *scan_row(char *str, size_t *n) {
	assert(str != NULL && n != NULL);

	if (*n == 0) {
		*n = count_tokens(str, " ");
	}

	T *ret = (T *)malloc(sizeof(ret[0]) * *n);
	assert(ret != NULL);

	char *save = NULL;
	char *token = strtok_r(str, " ", &save);
	assert(token != NULL);
	for (size_t i = 0; i < *n && token != NULL;
	     i++, token = strtok_r(NULL, " ", &save)) {
		sscanf(token, "%d", &ret[i]);
	}

	return ret;
}

template <typename T> void square_matrix<T>::from_string(char *str) {
	assert(str != NULL);

	char *token = strtok(str, ";");
	assert(token != NULL);
	this->size = 0;
	T *row1 = scan_row<T>(token, &this->size);
	this->rows = (T **)malloc(sizeof(this->rows[0]) * this->size);
	assert(this->rows != NULL);
	this->rows[0] = row1;

	token = strtok(NULL, ";");
	for (size_t i = 1; i < this->size && token != NULL;
	     i++, token = strtok(NULL, ";")) {
		this->rows[i] = scan_row<T>(token, &this->size);
	}
}

template <typename T>
void square_matrix<T>::add(const square_matrix<T> *other) {
	assert(this->size == other->size);
	assert(this->rows != NULL && other->rows != NULL);

	for (size_t i = 0; i < this->size; i++) {
		assert(this->rows[i] != NULL && other->rows[i] != NULL);
		for (size_t j = 0; j < this->size; j++) {
			this->rows[i][j] += other->rows[i][j];
		}
	}
}

template <typename T>
void square_matrix<T>::sub(const square_matrix<T> *other) {
	assert(this->size == other->size);
	assert(this->rows != NULL && other->rows != NULL);

	for (size_t i = 0; i < this->size; i++) {
		assert(this->rows[i] != NULL && other->rows[i] != NULL);
		for (size_t j = 0; j < this->size; j++) {
			this->rows[i][j] -= other->rows[i][j];
		}
	}
}

template <typename T> void square_matrix<T>::mul(T num) {
	assert(this->rows != NULL);

	for (size_t i = 0; i < this->size; i++) {
		assert(this->rows[i] != NULL);
		for (size_t j = 0; j < this->size; j++) {
			this->rows[i][j] *= num;
		}
	}
}

template <typename T>
static T linear_combination(std::function<T(size_t)> set1_subscript,
			    std::function<T(size_t)> set2_subscript,
			    size_t len) {
	T acc = 0;
	for (size_t i = 0; i < len; i++) {
		acc += set1_subscript(i) * set2_subscript(i);
	}
	return acc;
}

template <typename T>
void square_matrix<T>::mul(const square_matrix<T> *other) {
	assert(this->rows && this->size);
	assert(other->rows && other->size);
	assert(this->size == other->size);

	square_matrix<T> tmp(*this);

	for (size_t i = 0; i < this->size; i++) {
		assert(this->rows[i] && other->rows[i]);
		for (size_t j = 0; j < this->size; j++) {
			this->rows[i][j] = linear_combination(
				(std::function<T(size_t)>)[
					i, tmp
				](size_t idx)->T { return tmp.rows[i][idx]; },
				(std::function<T(size_t)>)[ j, other ](
					size_t idx)->T {
					return other->rows[idx][j];
				},
				this->size);
		}
	}
}

template <typename T>
static bool is_element(T elem, const T arr[], size_t len) {
	assert(arr != NULL);

	for (size_t i = 0; i < len; i++) {
		if (elem == arr[i]) {
			return true;
		}
	}

	return false;
}

template <typename T>
square_matrix<T> *square_matrix<T>::submatrix(const size_t rows[],
					      const size_t cols[],
					      size_t rowccolc) {
	assert(rows != NULL && cols != NULL);
	assert(rowccolc <= this->size);
	assert(this->rows != NULL);

	square_matrix<T> *subm = new square_matrix<T>(rowccolc);
	assert(subm->rows != NULL);

	DPRINTF("submatrix: rowccolc: %zu\n", rowccolc);

	size_t ii = 0;
	for (size_t i = 0; i < this->size; i++) {
		assert(this->rows[i] != NULL);
		if (is_element(i, rows, rowccolc)) {
			assert(subm->rows[ii] != NULL);
			size_t jj = 0;
			for (size_t j = 0; j < this->size; j++) {
				if (is_element(j, cols, rowccolc)) {
					DPRINTF("submatrix: adding %d\n",
						this->rows[i][j]);
					subm->rows[ii][jj] = this->rows[i][j];
					++jj;
				}
			}
			++ii;
		}
	}

#ifdef DEBUG
	char *str = subm->to_string();
	DPRINTF("%s", str);
	free(str);
#endif

	return subm;
}

static size_t *range(size_t l, size_t h) {
	assert(h > l);
	size_t *ret = (size_t *)malloc(sizeof(ret[0]) * (h - l));
	assert(ret != NULL);

	for (size_t i = 0; l < h; i++, l++) {
		ret[i] = l;
	}

	return ret;
}

template <typename T> static T exp(T base, size_t n) {
	DPRINTF("exponentiation function called\n");
	switch (n) {
		case 0:
			fprintf(stderr, "a zero power would be a special case, "
					"not implemented\n");
			exit(1);
		case 1:
			return base;
	}
	if (n % 2 == 0) {
		return exp(base * base, n / 2);
	} else {
		return base * exp(base * base, (n - 1) / 2);
	}
}

static int single(size_t pow) {
	char s[200];
	assert(scanf("%199[^\n]s", s) > 0);
	square_matrix<int> m(s);

	char *m_s = m.to_string();
	printf("performing exponentiation (n: %zu) of base matrix "
	       "(size: %zu):\n%s",
	       pow, m.get_size(), m_s);
	free(m_s);

	square_matrix<int> p = exp<square_matrix<int>>(m, pow);
	// square_matrix<int> p = &m;

	m_s = p.to_string();
	printf("result:\n%s", m_s);
	free(m_s);

	return 0;
}

template <typename T> static square_matrix<T> gen_matrix(size_t msize) {
	return square_matrix<T>(msize);
}

static int test() {
	size_t msizes[] = {2, 4, 8, 16, 32, 64, 128};
	size_t msize_cnt = sizeof(msizes) / sizeof(msizes[0]);

	for (size_t i = 0; i < msize_cnt; i++) {
		square_matrix<int> m = gen_matrix<int>(msizes[i]);

		printf("size: %zu, ", msizes[i]);
		fflush(stdout);

		square_matrix<int> tmp = exp(m, 20);

		printf("did something\n");
	}

	return 0;
}

int main(int argc, char *argv[]) {
	if (argc == 1) {
		return test();
	} else if (argc == 2) {
		return single(atoll(argv[1]));
	} else {
		fprintf(stderr, "invalid args\n");
		return EXIT_FAILURE;
	}
}
