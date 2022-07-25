#define UNUSED(x) (void)(x)

int read_tsv_file(const char *filename, double *energy, double *values, double xunit, double yunit);
int partition(double arr[], int start, int end);
void quickSort(double arr[], int start, int end);
