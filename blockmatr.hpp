#include <iostream>


template<class T> class blockmatrix {
public:
    blockmatrix(size_t lines, size_t columns): lines(lines), columns(columns) { // subject to change
        bigbody = new T[lines * columns];
        body = new T*[lines];
        for (size_t i = 0; i < lines; i++) {
            body[i] = bigbody + i*columns;
        }
    }

    ~blockmatrix() {
        delete [] body;
        delete [] bigbody;
    }

    T * operator()(size_t line, size_t col) {
        return body[line]+col;
    }

    T * operator()(size_t line, size_t col) const {
        return body[line]+col;
    }


    bool mul(blockmatrix const &left, blockmatrix const &right) {
        //auto righ_t = right.transpose();
        if (left.columns != right.lines) return false;
        if (lines != left.lines) return false;
        if (columns != right.columns) return false;
        // transpose
        for (size_t i = 0; i < lines; i++) {
            for (size_t j = 0; j < columns; j++) {
                T acc = (T) 0;
                for (size_t k = 0; k < left.columns; k++) {
                    acc += *left(i,k) * *right(k, j);
                }
                *(*this)(i,j) = acc;
            }
        }
        return true;
    }

    bool mul_t(blockmatrix const &left, blockmatrix const &right) {
        //printf("mul: lines %d columns %d left (%d %d) right (%d %d)\n", lines, columns, left.lines, left.columns, right.lines, right.columns);
        if (left.columns != right.lines) return false;
        if (lines != left.lines) return false;
        if (columns != right.columns) return false;
        //printf("_mul: lines %d columns %d left (%d %d) right (%d %d)\n", lines, columns, left.lines, left.columns, right.lines, right.columns);

        int n = left.lines;
        int m = left.columns;
        int p = right.columns;

        if(n <= 800 && m <= 800 && p <= 800){  // align values to your cache size
            // just transposing and multiplying
            //printf("!mul: %d %d\n", lines, columns);
            blockmatrix right_t(right.columns, right.lines);
            for (int i = 0; i < right.lines; i++){
                T* r_pointer = right(i,0);
                for (int j = 0; j < right.columns; j++) {
                    *right_t(j, i) = *r_pointer;
                    r_pointer++;
                }
            }

            T* c_pointer = (*this)(0, 0);
            for (size_t i = 0; i < lines; i++) {
                for (size_t j = 0; j < columns; j++) {
                    T acc = (T) 0;
                    T* l_pointer = left(i,0);
                    T* r_pointer = right_t(j,0);

                    for (size_t k = 0; k < left.columns; k++) {
                        acc += *l_pointer * *r_pointer;
                        l_pointer++;
                        r_pointer++;
                    }
                    *c_pointer++ = acc;
                }
            }
            return true;
        }

        if(n >= m && n >= p){ // splitting left horizontally
            //printf("splitting left horizontally\n");
            blockmatrix top(n/2, m);
            blockmatrix bot(n/2, m);
            blockmatrix res_top(n/2, p);
            blockmatrix res_bot(n/2, p);

            top.copy(left, 0, 0);
            bot.copy(left, n/2, 0);
            res_top.mul_t(top, right);
            res_bot.mul_t(bot, right);
            this->compose(res_top, 0, 0);
            this->compose(res_bot, n/2, 0);
            return true;
        }
        else if(p >= m && p >= n){ //splitting right vertically
            //printf("splitting right vertically\n");
            blockmatrix lft(m, p/2);
            blockmatrix rght(m, p/2);
            blockmatrix res_lft(n, p/2);
            blockmatrix res_rght(n, p/2);
            lft.copy(right, 0, 0);
            rght.copy(right, 0, p/2);
            res_lft.mul_t(left, lft);
            res_rght.mul_t(left, rght);
            this->compose(res_lft, 0, 0);
            this->compose(res_rght, 0, p/2);
            return true;

        }
        else if(m >= n && m >= p){
            //printf("splitting left vertically, right horizontally\n");
            blockmatrix lft(n, m/2);
            blockmatrix rght(n, m/2);

            blockmatrix top(m/2, p);
            blockmatrix bottom(m/2, p);

            blockmatrix res1(n, p);
            blockmatrix res2(n, p);

            top.copy(right, 0, 0);
            bottom.copy(right, m/2, 0);
            lft.copy(left, 0, 0);
            rght.copy(left, 0, m/2);

            res1.mul_t(lft, top);
            res2.mul_t(rght, bottom);

            this->add(res1, res2);
            return true;
        }
        printf("error\n");
        return false;
    }

    bool add(blockmatrix const &left, blockmatrix const &right){
        for (int i = 0; i < right.lines; i++){
            T* c_pointer = (*this)(i, 0);
            T* l_pointer = left(i,0);
            T* r_pointer = right(i,0);
            for (int j = 0; j < right.columns; j++) {
                *c_pointer = *r_pointer + *l_pointer;
                l_pointer++;
                r_pointer++;
                c_pointer++;
            }
        }
        return true;
    }

private:
    T ** body;
    T *bigbody;
    size_t lines, columns;

    void copy(blockmatrix const &parent, int i_start, int j_start){
        //printf("copy %d %d\n", lines, columns);
        for(int i = 0; i < lines; i++){
            T* c_pointer = (*this)(i, 0);
            T* p_pointer = parent(i_start + i, j_start);
            for(int j = 0; j < columns; j++){
                *c_pointer = *p_pointer;
                c_pointer++;
                p_pointer++;
            }
        }
    }
    void compose(blockmatrix const &child, int i_start, int j_start){
        //printf("compose %d %d\n", lines, columns);
        for(int i = 0; i < child.lines; i++){
            T* c_pointer = (*this)(i_start + i, j_start);
            T* p_pointer = child(i, 0);
            for(int j = 0; j < child.columns; j++){
                *c_pointer = *p_pointer;
                c_pointer++;
                p_pointer++;
            }
        }
    }
};

