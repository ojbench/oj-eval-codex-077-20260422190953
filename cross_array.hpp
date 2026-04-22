// Implementation of sjtu::CrossArray as specified in README
// No standard library containers; only raw pointers and manual memory management.

namespace sjtu {

class CrossArray {
private:
    int **lines_;       // array of pointers to int arrays; nullptr means not inserted
    int *sizes_;        // sizes for each line; 0 if nullptr
    int capacity_;      // maximum number of lines (arrays in parallel)
    int used_;          // number of inserted arrays (non-null entries)

public:
    explicit CrossArray(int lines)
        : lines_(nullptr), sizes_(nullptr), capacity_(lines), used_(0) {
        if (capacity_ < 0) capacity_ = 0;
        lines_ = capacity_ ? new int*[capacity_] : nullptr;
        sizes_ = capacity_ ? new int[capacity_] : nullptr;
        for (int i = 0; i < capacity_; ++i) {
            lines_[i] = nullptr;
            sizes_[i] = 0;
        }
    }

    CrossArray(const CrossArray & o)
        : lines_(nullptr), sizes_(nullptr), capacity_(o.capacity_), used_(o.used_) {
        lines_ = capacity_ ? new int*[capacity_] : nullptr;
        sizes_ = capacity_ ? new int[capacity_] : nullptr;
        for (int i = 0; i < capacity_; ++i) {
            sizes_[i] = o.sizes_[i];
            if (o.lines_[i] == nullptr) {
                lines_[i] = nullptr;
            } else {
                int n = o.sizes_[i];
                lines_[i] = n ? new int[n] : nullptr;
                for (int j = 0; j < n; ++j) lines_[i][j] = o.lines_[i][j];
            }
        }
    }

    CrossArray & WhichGreater(CrossArray & o) {
        long long cnt_this = 0, cnt_o = 0;
        for (int i = 0; i < capacity_; ++i) cnt_this += sizes_[i];
        for (int i = 0; i < o.capacity_; ++i) cnt_o += o.sizes_[i];
        return (cnt_this >= cnt_o) ? *this : o;
    }

    bool IsSame(const CrossArray & o) {
        // same space means underlying pointer arrays are the same address
        return lines_ == o.lines_;
    }

    bool InsertArrays(const int * Input, int size) {
        if (used_ >= capacity_) return false;
        // find first nullptr slot
        int idx = -1;
        for (int i = 0; i < capacity_; ++i) {
            if (lines_[i] == nullptr) { idx = i; break; }
        }
        if (idx == -1) return false; // all occupied but used_ tracking says full
        lines_[idx] = size ? new int[size] : nullptr;
        sizes_[idx] = size;
        for (int j = 0; j < size; ++j) lines_[idx][j] = Input[j];
        ++used_;
        return true;
    }

    void AppendArrays(const int * Input, int Line, int size) {
        // append Input to existing line
        int oldn = sizes_[Line];
        int newn = oldn + size;
        int *newarr = newn ? new int[newn] : nullptr;
        for (int j = 0; j < oldn; ++j) newarr[j] = lines_[Line][j];
        for (int j = 0; j < size; ++j) newarr[oldn + j] = Input[j];
        if (lines_[Line]) delete[] lines_[Line];
        lines_[Line] = newarr;
        sizes_[Line] = newn;
    }

    void DoubleCrossLength() {
        int newcap = capacity_ * 2;
        int **newlines = newcap ? new int*[newcap] : nullptr;
        int *newsizes = newcap ? new int[newcap] : nullptr;
        for (int i = 0; i < newcap; ++i) {
            if (i < capacity_) {
                newlines[i] = lines_[i];
                newsizes[i] = sizes_[i];
            } else {
                newlines[i] = nullptr;
                newsizes[i] = 0;
            }
        }
        delete[] lines_;
        delete[] sizes_;
        lines_ = newlines;
        sizes_ = newsizes;
        capacity_ = newcap;
    }

    const int * AtArray(int i) {
        return lines_[i];
    }

    int & At(int i, int j) {
        return lines_[i][j];
    }

    ~CrossArray() {
        if (lines_) {
            for (int i = 0; i < capacity_; ++i) {
                if (lines_[i]) delete[] lines_[i];
            }
            delete[] lines_;
        }
        if (sizes_) delete[] sizes_;
    }
};

} // namespace sjtu

