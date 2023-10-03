#include <assert.h>
#include <cstddef>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IS_A_POWER_OF_TWO(x) x != 0 && (x & (x - 1)) == 0

template <typename T> class square_matrix {
	public:
	size_t size;
	void add(const square_matrix<T> *);
	void sub(const square_matrix<T> *);
	void mul(T);
	square_matrix<T> submatrix(const size_t[], const size_t[], size_t);

	char *to_string();
	void from_string(char *);

	T **rows;
};

template <typename T> char *square_matrix<T>::to_string() {
	assert(this->rows != NULL);
	char *ret = (char *)malloc(sizeof(char) * 1024);
	assert(ret != NULL);

	char *pos = ret;
	for (size_t i = 0; i < this->size; i++) {
		assert(this->rows[i] != NULL);
		for (size_t j = 0; j < this->size; j++) {
			pos += sprintf(pos, "%d%c", this->rows[i][j],
				       j == this->size - 1 ? '\n' : ' ');
		}
	}

	return ret;
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

template <typename T> T *scan_row(char *str, size_t *n) {
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
square_matrix<T> square_matrix<T>::submatrix(const size_t rows[],
					     const size_t cols[],
					     size_t rowccolc) {
	assert(rows != NULL && cols != NULL);
	assert(rowccolc <= this->size);
	assert(this->rows != NULL);

	square_matrix<T> subm;
	subm.size = rowccolc;
	subm.rows = (T **)malloc(sizeof(subm.rows[0]) * subm.size);
	assert(subm.rows != NULL);

	fprintf(stderr, "submatrix: rowccolc: %zu\n", rowccolc);

	size_t ii = 0;
	for (size_t i = 0; i < this->size; i++) {
		assert(this->rows[i] != NULL);
		if (is_element(i, rows, rowccolc)) {
			subm.rows[ii] = (T *)malloc(sizeof(subm.rows[0][0]) *
						    subm.size);
			assert(subm.rows[ii] != NULL);
			size_t jj = 0;
			for (size_t j = 0; j < this->size; j++) {
				if (is_element(j, cols, rowccolc)) {
					fprintf(stderr,
						"submatrix: adding %d\n",
						this->rows[i][j]);
					subm.rows[ii][jj] = this->rows[i][j];
					++jj;
				}
			}
			++ii;
		}
	}

	fprintf(stderr, "%s", subm.to_string());

	return subm;
}

template <typename T> class bapprox_matrix : public square_matrix<T> {
	public:
	void from_string(char *);
	void from_square_matrix(square_matrix<T>);
	bapprox_matrix<T> block_approx(size_t);

	private:
	T avg();
	bapprox_matrix<T> get_block(size_t, size_t, size_t, size_t);
};

template <typename T> void bapprox_matrix<T>::from_string(char *str) {
	square_matrix<T>::from_string(str);

	assert(IS_A_POWER_OF_TWO(this->size));
}

template <typename T>
void bapprox_matrix<T>::from_square_matrix(square_matrix<T> sq_mx) {
	fprintf(stderr, "size: %zu\n", sq_mx.size);
	assert(IS_A_POWER_OF_TWO(sq_mx.size));
	this->size = sq_mx.size;
	this->rows = sq_mx.rows;

	fprintf(stderr, "%s", sq_mx.to_string());
	fprintf(stderr, "%s", this->to_string());
}

template <typename T> T bapprox_matrix<T>::avg() {
	assert(this->rows != NULL);
	fprintf(stderr, "avg: size: %zu\n", this->size);
	T sum = 0;

	for (size_t i = 0; i < this->size; i++) {
		for (size_t j = 0; j < this->size; j++) {
			fprintf(stderr, "avg: adding %d\n", this->rows[i][j]);
			sum += this->rows[i][j];
		}
	}

	fprintf(stderr, "avg: sum: %d\n", sum);

	return sum / (this->size * this->size);
}

static size_t *range(size_t l, size_t h) {
	assert(h > l);
	size_t *ret = (size_t *)malloc(sizeof(ret[0]) * (h - l));
	assert(ret != NULL);

	for (size_t i = 0, n = l; l < h; i++, l++) {
		ret[i] = l;
	}

	return ret;
}

template <typename T>
bapprox_matrix<T> bapprox_matrix<T>::get_block(size_t rowl, size_t rowh,
					       size_t coll, size_t colh) {
	assert(rowh > rowl && colh > coll);
	assert(rowh - rowl == colh - coll);
	assert(this->size > 0 && this->rows != NULL);

	fprintf(stderr,
		"get_block: rowl: %zu, rowh: %zu, coll: %zu, colh: %zu\n", rowl,
		rowh, coll, colh);

	size_t *rows = range(rowl, rowh);
	size_t *cols = range(coll, colh);
	assert(rows != NULL && cols != NULL);

	fprintf(stderr, "get_block: subm size: %zu\n", rowh - rowl);

	bapprox_matrix<T> ret;
	ret.from_square_matrix(
		square_matrix<T>::submatrix(rows, cols, rowh - rowl));

	free(rows);
	free(cols);

	return ret;
}

template <typename T>
bapprox_matrix<T> bapprox_matrix<T>::block_approx(size_t bsize) {
	assert(this->size >= bsize);
	assert(IS_A_POWER_OF_TWO(bsize));

	bapprox_matrix<T> apprm;
	apprm.size = this->size / bsize;
	assert(apprm.size >= 1);
	apprm.rows = (T **)malloc(sizeof(apprm.rows[0]) * apprm.size);
	assert(apprm.rows != NULL);

	for (size_t i = 0; i < this->size / bsize; i++) {
		apprm.rows[i] =
			(T *)malloc(sizeof(apprm.rows[0][0]) * apprm.size);
		assert(apprm.rows[i] != NULL);

		for (size_t j = 0; j < this->size / bsize; j++) {
			apprm.rows[i][j] =
				this->get_block(i * bsize, (i + 1) * bsize,
						j * bsize, (j + 1) * bsize)
					.avg();
		}
	}

	return apprm;
}

int main(int argc, char *argv[]) {
	assert(argc == 2);

	bapprox_matrix<int> m;

	char s[100];
	assert(scanf("%99[^\n]s", s) > 0);
	m.from_string(s);

	printf("%s", m.to_string());

	printf("%s", m.block_approx(1ULL << atoll(argv[1])).to_string());
}
