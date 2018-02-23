// функция расчёта поворотных множителей для БПФ
void fft_make(
  int p,     // показатель двойки (например, для БПФ на 256 точек это 8)
  float *c); // результирующий массив поворотных множителей c[1 << p]
//----------------------------------------------------------------------------
// функция расчёта поворотных множителей для ОБПФ
void fft_make_reverse(
  int p,     // показатель двойки (например, для ОБПФ на 256 точек это 8)
  float *c); // результирующий массив поворотных множителей c[1 << p]
//----------------------------------------------------------------------------
// функция бинарной инверсии индекса
int fft_binary_inversion(
	int p, // показатель двойки (например, для БПФ на 256 точек это 8)
	int i); // исходный индекс [0, (1 << p) - 1]

//----------------------------------------------------------------------------
 void fft_calc(
	 int p,  // показатель двойки (например, для БПФ на 256 точек это 8)
	  const float *c,  // массив поворотных множителей ОБПФ
	  const float *in, // входной массив
	  float *out,      // выходной массив
	  int norm);       // признак нормировки
//---------------------------------------------------------------------------
// функция перестановки отсчётов спектра (что бы "0" в центре)
void fft_shift(
  int p,        // показатель двойки (например, для БПФ на 256 точек это 8)
  float *data); // массив после БПФ