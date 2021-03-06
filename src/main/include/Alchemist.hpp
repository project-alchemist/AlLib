#ifndef ALCHEMIST_HPP
#define ALCHEMIST_HPP

#include <string>
#include "mpi.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace alchemist {

using std::string;
using std::stringstream;
using std::vector;

#define SPACE "                                              ";
#define TAB "    ";

typedef enum _datatype : uint8_t {
	NONE = 0,
	CHAR,
	SIGNED_CHAR,
	UNSIGNED_CHAR,
	CHARACTER,
	WCHAR,
	SHORT,
	UNSIGNED_SHORT,
	INT,
	UNSIGNED,
	LONG,
	UNSIGNED_LONG,
	LONG_LONG_INT,
	LONG_LONG,
	UNSIGNED_LONG_LONG,
	FLOAT,
	DOUBLE,
	LONG_DOUBLE,
	BYTE,
	BOOL,
	INTEGER,
	REAL,
	LOGICAL,
	COMPLEX,
	DOUBLE_PRECISION,
	REAL4,
	COMPLEX8,
	REAL8,
	COMPLEX16,
	INTEGER1,
	INTEGER2,
	INTEGER4,
	INTEGER8,
	INT8,
	INT16,
	INT32,
	INT64,
	UINT8,
	UINT16,
	UINT32,
	UINT64,
	FLOAT_INT,
	DOUBLE_INT,
	LONG_INT,
	SHORT_INT,
	LONG_DOUBLE_INT,
	STRING,
	WSTRING,
	COMMAND_CODE,
	LIBRARY_ID,
	GROUP_ID,
	WORKER_ID,
	WORKER_INFO,
	MATRIX_ID,
	MATRIX_INFO,
	MATRIX_BLOCK,
	INDEXED_ROW,
	DISTMATRIX,
	ARRAY_ID,
	ARRAY_INFO,
	ARRAY_BLOCK_FLOAT,
	ARRAY_BLOCK_DOUBLE,
	VOID_POINTER,
	DISTMATRIX_MC_MR,
	DISTMATRIX_MC_STAR,
	DISTMATRIX_MD_STAR,
	DISTMATRIX_MR_MC,
	DISTMATRIX_MR_STAR,
	DISTMATRIX_STAR_MC,
	DISTMATRIX_STAR_MD,
	DISTMATRIX_STAR_MR,
	DISTMATRIX_STAR_STAR,
	DISTMATRIX_STAR_VC,
	DISTMATRIX_STAR_VR,
	DISTMATRIX_VC_STAR,
	DISTMATRIX_VR_STAR,
	DISTMATRIX_CIRC_CIRC,
	PARAMETER = 100
} datatype;

typedef uint16_t Matrix_ID;
typedef uint16_t Worker_ID;

// =================================================================================================
// ========================================= Logging ===============================================
// =================================================================================================

// Formatting codes
const string regular = "";
const string reset = "\033[m";
const string bold = "\033[1m";
const string dark = "\033[2m";
const string italic = "\033[3m";
const string underline = "\033[4m";
const string blink = "\033[5m";
const string reverse = "\033[7m";
const string concealed = "\033[8m";
const string clear_line = "\033[K";

// Foreground colors
const string black = "\033[30m";
const string red = "\033[31m";
const string green = "\033[32m";
const string yellow = "\033[33m";
const string blue = "\033[34m";
const string magenta = "\033[35m";
const string cyan = "\033[36m";
const string white = "\033[37m";
const string iblack = "\033[90m";
const string ired = "\033[91m";
const string igreen = "\033[92m";
const string iyellow = "\033[93m";
const string iblue = "\033[94m";
const string ipurple = "\033[95m";
const string icyan = "\033[96m";
const string iwhite = "\033[97m";

// Background colors
const string on_black = "\033[40m";
const string on_red = "\033[41m";
const string on_green = "\033[42m";
const string on_yellow = "\033[43m";
const string on_blue = "\033[44m";
const string on_magenta = "\033[45m";
const string on_cyan = "\033[46m";
const string on_white = "\033[47m";
const string on_iblack = "\033[100m";
const string on_ired = "\033[101m";
const string on_igreen = "\033[102m";
const string on_iyellow = "\033[103m";
const string on_iblue = "\033[104m";
const string on_ipurple = "\033[105m";
const string on_icyan = "\033[106m";
const string on_iwhite = "\033[107m";

typedef std::shared_ptr<spdlog::logger> Log_ptr;

inline Log_ptr start_log(string name, string pattern, string format=regular, string fore_color="", string back_color="")
{
	string logfile_name = name + ".log";

	auto console_sink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_st>();
	console_sink->set_level(spdlog::level::info);
	console_sink->set_pattern(pattern);
	console_sink->set_color(spdlog::level::info, format + fore_color + back_color);

	auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_st>(logfile_name, true);
	file_sink->set_level(spdlog::level::trace);

	Log_ptr log;
	std::vector<spdlog::sink_ptr> sinks;
	sinks.push_back(console_sink);
	sinks.push_back(file_sink);
	log = std::make_shared<spdlog::logger>(name, std::begin(sinks), std::end(sinks));
	return log;
}

// =================================================================================================
// ========================================= Matrices ==============================================
// =================================================================================================

struct MatrixInfo {
	Matrix_ID ID;

	string name;
	uint64_t num_rows, num_cols;
	bool sparse;
	uint8_t layout, num_partitions;

	Worker_ID * row_assignments;

	explicit MatrixInfo() : ID(0), name(""), num_rows(1), num_cols(1), sparse(false), layout(0), num_partitions(0), row_assignments(nullptr) {
		row_assignments = new Worker_ID[num_rows]();
	}

	MatrixInfo(Matrix_ID _ID, uint64_t _num_rows, uint64_t _num_cols) :
		ID(_ID), name(""), num_rows(_num_rows), num_cols(_num_cols), sparse(false), layout(0), num_partitions(0), row_assignments(nullptr) {
		row_assignments = new Worker_ID[num_rows]();
	}

	MatrixInfo(Matrix_ID _ID, string _name, uint64_t _num_rows, uint64_t _num_cols) :
		ID(_ID), name(_name), num_rows(_num_rows), num_cols(_num_cols), sparse(false), layout(0), num_partitions(0), row_assignments(nullptr) {
		row_assignments = new Worker_ID[num_rows]();
	}
	MatrixInfo(Matrix_ID _ID, string _name, uint64_t _num_rows, uint64_t _num_cols, bool _sparse, uint8_t _layout, uint8_t _num_partitions) :
		ID(_ID), name(_name), num_rows(_num_rows), num_cols(_num_cols), sparse(_sparse), layout(0), num_partitions(_num_partitions), row_assignments(nullptr) {
		std::cout << "SHFHSH" << std::endl;
		row_assignments = new Worker_ID[num_rows]();
		std::cout << "SHFHSH gg" << std::endl;
	}

	~MatrixInfo() {
		delete [] row_assignments; row_assignments = nullptr;
	}

	string to_string(bool display_layout=false) const {
		stringstream ss;

		ss << "Matrix " << name << " (ID: " << ID << ", dim: " << num_rows << " x " << num_cols << ", sparse: " << (uint16_t) sparse << ", # partitions: " << (uint16_t) num_partitions << ")";
		if (display_layout) {
			ss << std::endl << "Layout: " << std::endl;
			for (uint64_t i = 0; i < num_rows; i++) ss << (uint16_t) row_assignments[i] << " ";
		}

		return ss.str();
	}
};

typedef std::shared_ptr<MatrixInfo> MatrixInfo_ptr;

typedef El::DistMatrix<double> DistMatrix;
typedef std::shared_ptr<El::AbstractDistMatrix<double>> DistMatrix_ptr;

// =================================================================================================
// ======================================== Parameters =============================================
// =================================================================================================

struct Parameter {
	Parameter(string _name, datatype _dt, void * _p) : name(_name), dt(_dt), p(_p) { }

	string name;
	datatype dt;
	void * p;
};

typedef std::shared_ptr<Parameter> Parameter_ptr;

// =================================================================================================
// ========================================== Library ==============================================
// =================================================================================================

struct Library {

	Library(MPI_Comm & _world) : world(_world) { }

	virtual ~Library() {}

	MPI_Comm & world;

	Log_ptr log;

	virtual int load() = 0;
	virtual int unload() = 0;
	virtual int run(string & task_name, std::vector<Parameter_ptr> & in, std::vector<Parameter_ptr> & out) = 0;
};

typedef std::shared_ptr<Library> Library_ptr;

#ifdef __cplusplus
extern "C" {
#endif

void * create_library(MPI_Comm &);
void destroy_library(void *);

#ifdef __cplusplus
}
#endif


}

#endif

































//struct Parameter {
//public:
//	Parameter(string _name, datatype _dt) : name(_name), dt(_dt) { }
//
//	virtual ~Parameter() {}
//
//	datatype dt;
//
//	string get_name() const {
//		return name;
//	}
//
//	virtual string to_string() const = 0;
//protected:
//	string name;
//};
//
//struct CharParameter : Parameter {
//public:
//	CharParameter(string _name, char _value) : Parameter(_name, CHAR), value(_value) { }
//
//	~CharParameter() { }
//
//	char get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		string s = "" + value;
//		return s;
//	}
//
//protected:
//	char value;
//};
//
//struct SignedCharParameter : Parameter {
//public:
//	SignedCharParameter(string _name, signed char _value) : Parameter(_name, SIGNED_CHAR), value(_value) { }
//
//	~SignedCharParameter() {}
//
//	signed char get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		string s = "" + value;
//		return s;
//	}
//
//protected:
//	signed char value;
//};
//
//struct UnsignedCharParameter : Parameter {
//public:
//	UnsignedCharParameter(string _name, unsigned char _value) : Parameter(_name, UNSIGNED_CHAR), value(_value) { }
//
//	~UnsignedCharParameter() {}
//
//	unsigned char get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		string s = "" + value;
//		return s;
//	}
//
//protected:
//	unsigned char value;
//};
//
//struct CharacterParameter : Parameter {
//public:
//	CharacterParameter(string _name, char _value) : Parameter(_name, CHARACTER), value(_value) { }
//
//	~CharacterParameter() {}
//
//	char get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		string s = "" + value;
//		return s;
//	}
//
//protected:
//	char value;
//};
//
//struct WcharParameter : Parameter {
//public:
//	WcharParameter(string _name, wchar_t _value) : Parameter(_name, WCHAR), value(_value) { }
//
//	~WcharParameter() {}
//
//	wchar_t get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		string s = "" + value;
//		return s;
//	}
//
//protected:
//	wchar_t value;
//};
//
//struct ShortParameter : Parameter {
//public:
//	ShortParameter(string _name, short _value) : Parameter(_name, SHORT), value(_value) { }
//
//	~ShortParameter() {}
//
//	short get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	short value;
//};
//
//struct UnsignedShortParameter : Parameter {
//public:
//	UnsignedShortParameter(string _name, unsigned short _value) : Parameter(_name, UNSIGNED_SHORT), value(_value) { }
//
//	~UnsignedShortParameter() {}
//
//	unsigned short get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	unsigned short value;
//};
//
//struct IntParameter : Parameter {
//public:
//	IntParameter(string _name, int _value) : Parameter(_name, INT), value(_value) { }
//
//	~IntParameter() {}
//
//	int get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	int value;
//};
//
//struct IntegerParameter : Parameter {
//public:
//	IntegerParameter(string _name, int _value) : Parameter(_name, INTEGER), value(_value) { }
//
//	~IntegerParameter() {}
//
//	int get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	int value;
//};
//
//struct UnsignedParameter : Parameter {
//public:
//	UnsignedParameter(string _name, unsigned int _value) : Parameter(_name, UNSIGNED), value(_value) { }
//
//	~UnsignedParameter() {}
//
//	unsigned int get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	unsigned int value;
//};
//
//struct LongParameter : Parameter {
//public:
//	LongParameter(string _name, long _value) : Parameter(_name, LONG), value(_value) { }
//
//	~LongParameter() {}
//
//	long get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	long value;
//};
//
//struct UnsignedLongParameter : Parameter {
//public:
//	UnsignedLongParameter(string _name, unsigned long _value) : Parameter(_name, UNSIGNED_LONG), value(_value) { }
//
//	~UnsignedLongParameter() {}
//
//	unsigned long get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	unsigned long value;
//};
//
//struct LongLongIntParameter : Parameter {
//public:
//	LongLongIntParameter(string _name, long long int _value) : Parameter(_name, LONG_LONG_INT), value(_value) { }
//
//	~LongLongIntParameter() {}
//
//	long long int get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	long long int value;
//};
//
//struct LongLongParameter : Parameter {
//public:
//	LongLongParameter(string _name, long long _value) : Parameter(_name, LONG_LONG), value(_value) { }
//
//	~LongLongParameter() {}
//
//	long long get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	long long value;
//};
//
//struct UnsignedLongLongParameter : Parameter {
//public:
//	UnsignedLongLongParameter(string _name, unsigned long long _value) : Parameter(_name, UNSIGNED_LONG_LONG), value(_value) { }
//
//	~UnsignedLongLongParameter() {}
//
//	unsigned long long get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	unsigned long long value;
//};
//
//struct FloatParameter : Parameter {
//public:
//	FloatParameter(string _name, float _value) : Parameter(_name, FLOAT), value(_value) { }
//
//	~FloatParameter() {}
//
//	float get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	float value;
//};
//
//struct DoubleParameter : Parameter {
//public:
//	DoubleParameter(string _name, double _value) : Parameter(_name, DOUBLE), value(_value) { }
//
//	~DoubleParameter() {}
//
//	double get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	double value;
//};
//
//struct LongDoubleParameter : Parameter {
//public:
//	LongDoubleParameter(string _name, long double _value) : Parameter(_name, LONG_DOUBLE), value(_value) { }
//
//	~LongDoubleParameter() {}
//
//	long double get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	long double value;
//};
//
//struct ByteParameter : Parameter {
//public:
//	ByteParameter(string _name, uint8_t _value) : Parameter(_name, BYTE), value(_value) { }
//
//	~ByteParameter() {}
//
//	uint8_t get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	uint8_t value;
//};
//
//struct BoolParameter : Parameter {
//public:
//	BoolParameter(string _name, bool _value) : Parameter(_name, BOOL), value(_value) { }
//
//	~BoolParameter() {}
//
//	bool get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	bool value;
//};
//
//struct RealParameter : Parameter {
//public:
//	RealParameter(string _name, double _value) : Parameter(_name, REAL), value(_value) { }
//
//	~RealParameter() {}
//
//	double get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	double value;
//};
//
//struct LogicalParameter : Parameter {
//public:
//	LogicalParameter(string _name, bool _value) : Parameter(_name, LOGICAL), value(_value) { }
//
//	~LogicalParameter() {}
//
//	bool get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	bool value;
//};
//
//struct ComplexParameter : Parameter {
//public:
//	ComplexParameter(string _name, std::complex<double> _value) : Parameter(_name, COMPLEX), value(0) { value = _value; }
//
//	~ComplexParameter() {}
//
//	std::complex<double> get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		stringstream ss;
//
//		ss << value;
//
//		return ss.str();
//	}
//
//protected:
//	std::complex<double> value;
//};
//
//struct DoublePrecisionParameter : Parameter {
//public:
//	DoublePrecisionParameter(string _name, double _value) : Parameter(_name, DOUBLE_PRECISION), value(_value) { }
//
//	~DoublePrecisionParameter() {}
//
//	double get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	double value;
//};
//
//struct Real4Parameter : Parameter {
//public:
//	Real4Parameter(string _name, float _value) : Parameter(_name, REAL4), value(_value) { }
//
//	~Real4Parameter() {}
//
//	float get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	float value;
//};
//
//struct Real8Parameter : Parameter {
//public:
//	Real8Parameter(string _name, double _value) : Parameter(_name, REAL8), value(_value) { }
//
//	~Real8Parameter() { }
//
//	double get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	double value;
//};
//
//struct Complex8Parameter : Parameter {
//public:
//	Complex8Parameter(string _name, std::complex<float> _value) : Parameter(_name, COMPLEX8), value(0) { value = _value; }
//
//	~Complex8Parameter() {}
//
//	std::complex<float> get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		stringstream ss;
//
//		ss << value;
//
//		return ss.str();
//	}
//
//protected:
//	std::complex<float> value;
//};
//
//struct Complex16Parameter : Parameter {
//public:
//	Complex16Parameter(string _name, std::complex<double> _value) : Parameter(_name, COMPLEX16), value(0) { value = _value; }
//
//	~Complex16Parameter() {}
//
//	std::complex<double> get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		stringstream ss;
//
//		ss << value;
//
//		return ss.str();
//	}
//
//protected:
//	std::complex<double> value;
//};
//
//struct Integer1Parameter : Parameter {
//public:
//	Integer1Parameter(string _name, int8_t _value) : Parameter(_name, INTEGER1), value(_value) { }
//
//	~Integer1Parameter() { }
//
//	int8_t get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	int8_t value;
//};
//
//struct Integer2Parameter : Parameter {
//public:
//	Integer2Parameter(string _name, int16_t _value) : Parameter(_name, INTEGER2), value(_value) { }
//
//	~Integer2Parameter() { }
//
//	int16_t get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	int16_t value;
//};
//
//struct Integer4Parameter : Parameter {
//public:
//	Integer4Parameter(string _name, int32_t _value) : Parameter(_name, INTEGER4), value(_value) { }
//
//	~Integer4Parameter() { }
//
//	int32_t get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	int32_t value;
//};
//
//struct Integer8Parameter : Parameter {
//public:
//	Integer8Parameter(string _name, int64_t _value) : Parameter(_name, INTEGER8), value(_value) { }
//
//	~Integer8Parameter() { }
//
//	int64_t get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	int64_t value;
//};
//
//struct Int8Parameter : Parameter {
//public:
//	Int8Parameter(string _name, int8_t _value) : Parameter(_name, INT8_T), value(_value) { }
//
//	~Int8Parameter() { }
//
//	int8_t get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	int8_t value;
//};
//
//struct Int16Parameter : Parameter {
//public:
//	Int16Parameter(string _name, int16_t _value) : Parameter(_name, INT16_T), value(_value) { }
//
//	~Int16Parameter() { }
//
//	int16_t get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	int16_t value;
//};
//
//struct Int32Parameter : Parameter {
//public:
//	Int32Parameter(string _name, int32_t _value) : Parameter(_name, INT32_T), value(_value) { }
//
//	~Int32Parameter() { }
//
//	int32_t get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	int32_t value;
//};
//
//struct Int64Parameter : Parameter {
//public:
//	Int64Parameter(string _name, int64_t _value) : Parameter(_name, INT64_T), value(_value) { }
//
//	~Int64Parameter() { }
//
//	int64_t get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	int64_t value;
//};
//
//struct UInt8Parameter : Parameter {
//public:
//	UInt8Parameter(string _name, uint8_t _value) : Parameter(_name, UINT8_T), value(_value) { }
//
//	~UInt8Parameter() { }
//
//	uint8_t get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	uint8_t value;
//};
//
//struct UInt16Parameter : Parameter {
//public:
//	UInt16Parameter(string _name, uint16_t _value) : Parameter(_name, UINT16_T), value(_value) { }
//
//	~UInt16Parameter() { }
//
//	uint16_t get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	uint16_t value;
//};
//
//struct UInt32Parameter : Parameter {
//public:
//	UInt32Parameter(string _name, uint32_t _value) : Parameter(_name, UINT32_T), value(_value) { }
//
//	~UInt32Parameter() { }
//
//	uint32_t get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	uint32_t value;
//};
//
//struct UInt64Parameter : Parameter {
//public:
//	UInt64Parameter(string _name, uint64_t _value) : Parameter(_name, UINT64_T), value(_value) { }
//
//	~UInt64Parameter() { }
//
//	uint64_t get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	uint64_t value;
//};
//
//struct FloatIntParameter : Parameter {
//public:
//	FloatIntParameter(string _name, int32_t _value) : Parameter(_name, INT32_T), value(_value) { }
//
//	~FloatIntParameter() { }
//
//	int32_t get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	int32_t value;
//};
//
//struct DoubleIntParameter : Parameter {
//public:
//	DoubleIntParameter(string _name, int64_t _value) : Parameter(_name, INT64_T), value(_value) { }
//
//	~DoubleIntParameter() { }
//
//	int64_t get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	int64_t value;
//};
//
//struct ShortIntParameter : Parameter {
//public:
//	ShortIntParameter(string _name, int16_t _value) : Parameter(_name, INT16_T), value(_value) { }
//
//	~ShortIntParameter() { }
//
//	int16_t get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	int16_t value;
//};
//
//struct LongIntParameter : Parameter {
//public:
//	LongIntParameter(string _name, int64_t _value) : Parameter(_name, INT64_T), value(_value) { }
//
//	~LongIntParameter() { }
//
//	int64_t get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	int64_t value;
//};
//
//struct LongDoubleIntParameter : Parameter {
//public:
//	LongDoubleIntParameter(string _name, int64_t _value) : Parameter(_name, INT64_T), value(_value) { }
//
//	~LongDoubleIntParameter() { }
//
//	int64_t get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return std::to_string(value);
//	}
//
//protected:
//	int64_t value;
//};
//
//struct StringParameter : Parameter {
//public:
//	StringParameter(string _name, string _value) : Parameter(_name, STRING), value(_value) { }
//
//	~StringParameter() {}
//
//	string get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return value;
//	}
//
//protected:
//	string value;
//};
//
//struct WStringParameter : Parameter {
//public:
//	WStringParameter(string _name, string _value) : Parameter(_name, WSTRING), value(_value) { }
//
//	~WStringParameter() {}
//
//	string get_value() const {
//		return value;
//	}
//
//	string to_string() const {
////		stringstream ss;
////
////		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
////
////		ss << converter.to_bytes(value);
//
//		return value;
//	}
//
//protected:
//	string value;
//};
//
//struct MatrixInfoParameter : Parameter {
//public:
//
//	MatrixInfoParameter(string _name, MatrixInfo_ptr _value) : Parameter(_name, MATRIX_INFO), value(_value) { }
//
//	~MatrixInfoParameter() { }
//
//	MatrixInfo_ptr get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		return value->to_string();
//	}
//
//protected:
//	MatrixInfo_ptr value;
//};
//
//struct DistMatrixParameter : Parameter {
//public:
//
//	DistMatrixParameter(string _name, DistMatrix_ptr _value) : Parameter(_name, DISTMATRIX), value(_value) {}
//
//	~DistMatrixParameter() {}
//
//	DistMatrix_ptr get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		stringstream ss;
//		ss << value;
//		return ss.str();
//	}
//
//protected:
//	DistMatrix_ptr value;
//};
//
//struct PointerParameter : Parameter {
//public:
//
//	PointerParameter(string _name, void * _value) : Parameter(_name, VOID_POINTER), value(_value) {}
//
//	~PointerParameter() {}
//
//	void * get_value() const {
//		return value;
//	}
//
//	string to_string() const {
//		stringstream ss;
//		ss << value;
//		return ss.str();
//	}
//
//protected:
//	void * value;
//};
//
//struct Parameters {
//public:
//	Parameters() : current_parameter_count(0), current_distmatrix_count(0) { }
//
//	~Parameters() { }
//
//	std::vector<string> distmatrix_names;
//	std::vector<string> matrix_info_names;
//	std::vector<string> ptr_names;
//
//	uint32_t current_parameter_count;
//	std::map<string, std::shared_ptr<Parameter> >::iterator it;
//
//	uint8_t current_distmatrix_count;
//	std::vector<string>::iterator distmatrix_it;
//
//	datatype get_next_parameter()
//	{
//		datatype _dt = NONE;
//
//		if (current_parameter_count == 0) it = parameters.begin();
//		else it++;
//
//		if (it != parameters.end()) {
//			current_parameter_count++;
//			_dt = it->second->dt;
//		}
//
//		return _dt;
//	}
//
//	void get_next_distmatrix(string & distmatrix_name, DistMatrix_ptr & distmatrix_ptr)
//	{
//		distmatrix_name = "";
//		distmatrix_ptr = nullptr;
//
//		if (current_distmatrix_count == 0) distmatrix_it = distmatrix_names.begin();
//		else distmatrix_it++;
//
//		if (distmatrix_it != distmatrix_names.end()) {
//			current_distmatrix_count++;
//			distmatrix_name = *distmatrix_it;
//			distmatrix_ptr = get_distmatrix(*distmatrix_it);
//		}
//	}
//
//	string get_name() {
//		return it->second->get_name();
//	}
//
//	uint8_t num_distmatrices() {
//		return (uint8_t) distmatrix_names.size();
//	}
//
//	uint8_t num_matrix_infos() {
//		return (uint8_t) matrix_info_names.size();
//	}
//
//	uint8_t num_void_pointers() {
//		return (uint8_t) ptr_names.size();
//	}
//
//	int num() const {
//		return parameters.size();
//	}
//
////	size_t num_distmatrices() const {
////		return distmatrix_parameters.size();
////	}
//
//	void add(Parameter * p) {
//		parameters.insert(std::make_pair(p->get_name(), p));
//	}
//
//	std::shared_ptr<Parameter> get(string name) const {
//		return parameters.find(name)->second;
//	}
//
//	void add_char(string name, char value) {
//		parameters.insert(std::make_pair(name, new CharParameter(name, value)));
//	}
//
//	void add_signed_char(string name, signed char value) {
//		parameters.insert(std::make_pair(name, new SignedCharParameter(name, value)));
//	}
//
//	void add_unsigned_char(string name, unsigned char value) {
//		parameters.insert(std::make_pair(name, new UnsignedCharParameter(name, value)));
//	}
//
//	void add_character(string name, char value) {
//		parameters.insert(std::make_pair(name, new CharacterParameter(name, value)));
//	}
//
//	void add_wchar(string name, wchar_t value) {
//		parameters.insert(std::make_pair(name, new WcharParameter(name, value)));
//	}
//
//	void add_short(string name, short value) {
//		parameters.insert(std::make_pair(name, new ShortParameter(name, value)));
//	}
//
//	void add_unsigned_short(string name, unsigned short value) {
//		parameters.insert(std::make_pair(name, new UnsignedShortParameter(name, value)));
//	}
//
//	void add_int(string name, int value) {
//		parameters.insert(std::make_pair(name, new IntParameter(name, value)));
//	}
//
//	void add_unsigned(string name, unsigned int value) {
//		parameters.insert(std::make_pair(name, new UnsignedParameter(name, value)));
//	}
//
//	void add_long(string name, long value) {
//		parameters.insert(std::make_pair(name, new LongParameter(name, value)));
//	}
//
//	void add_unsigned_long(string name, unsigned long value) {
//		parameters.insert(std::make_pair(name, new UnsignedLongParameter(name, value)));
//	}
//
//	void add_long_long_int(string name, long long int value) {
//		parameters.insert(std::make_pair(name, new LongLongIntParameter(name, value)));
//	}
//
//	void add_long_long(string name, long long int value) {
//		parameters.insert(std::make_pair(name, new LongLongParameter(name, value)));
//	}
//
//	void add_unsigned_long_long(string name, unsigned long long int value) {
//		parameters.insert(std::make_pair(name, new UnsignedLongLongParameter(name, value)));
//	}
//
//	void add_float(string name, float value) {
//		parameters.insert(std::make_pair(name, new FloatParameter(name, value)));
//	}
//
//	void add_double(string name, double value) {
//		parameters.insert(std::make_pair(name, new DoubleParameter(name, value)));
//	}
//
//	void add_long_double(string name, long double value) {
//		parameters.insert(std::make_pair(name, new LongDoubleParameter(name, value)));
//	}
//
//	void add_byte(string name, uint8_t value) {
//		parameters.insert(std::make_pair(name, new ByteParameter(name, value)));
//	}
//
//	void add_bool(string name, bool value) {
//		parameters.insert(std::make_pair(name, new BoolParameter(name, value)));
//	}
//
//	void add_integer(string name, int value) {
//		parameters.insert(std::make_pair(name, new IntegerParameter(name, value)));
//	}
//
//	void add_real(string name, double value) {
//		parameters.insert(std::make_pair(name, new RealParameter(name, value)));
//	}
//
//	void add_logical(string name, bool value) {
//		parameters.insert(std::make_pair(name, new LogicalParameter(name, value)));
//	}
//
//	void add_complex(string name, std::complex<double> value) {
//		parameters.insert(std::make_pair(name, new ComplexParameter(name, value)));
//	}
//
//	void add_double_precision(string name, double value) {
//		parameters.insert(std::make_pair(name, new DoublePrecisionParameter(name, value)));
//	}
//
//	void add_real4(string name, float value) {
//		parameters.insert(std::make_pair(name, new Real4Parameter(name, value)));
//	}
//
//	void add_complex8(string name, std::complex<float> value) {
//		parameters.insert(std::make_pair(name, new Complex8Parameter(name, value)));
//	}
//
//	void add_real8(string name, double value) {
//		parameters.insert(std::make_pair(name, new Real8Parameter(name, value)));
//	}
//
//	void add_complex16(string name, std::complex<double> value) {
//		parameters.insert(std::make_pair(name, new Complex16Parameter(name, value)));
//	}
//
//	void add_integer1(string name, int8_t value) {
//		parameters.insert(std::make_pair(name, new Integer1Parameter(name, value)));
//	}
//
//	void add_integer2(string name, int16_t value) {
//		parameters.insert(std::make_pair(name, new Integer2Parameter(name, value)));
//	}
//
//	void add_integer4(string name, int32_t value) {
//		parameters.insert(std::make_pair(name, new Integer4Parameter(name, value)));
//	}
//
//	void add_integer8(string name, int64_t value) {
//		parameters.insert(std::make_pair(name, new Integer8Parameter(name, value)));
//	}
//
//	void add_int8(string name, int8_t value) {
//		parameters.insert(std::make_pair(name, new Int8Parameter(name, value)));
//	}
//
//	void add_int16(string name, int16_t value) {
//		parameters.insert(std::make_pair(name, new Int16Parameter(name, value)));
//	}
//
//	void add_int32(string name, int32_t value) {
//		parameters.insert(std::make_pair(name, new Int32Parameter(name, value)));
//	}
//
//	void add_int64(string name, int64_t value) {
//		parameters.insert(std::make_pair(name, new Int64Parameter(name, value)));
//	}
//
//	void add_uint8(string name, uint8_t value) {
//		parameters.insert(std::make_pair(name, new UInt8Parameter(name, value)));
//	}
//
//	void add_uint16(string name, uint16_t value) {
//		parameters.insert(std::make_pair(name, new UInt16Parameter(name, value)));
//	}
//
//	void add_uint32(string name, uint32_t value) {
//		parameters.insert(std::make_pair(name, new UInt32Parameter(name, value)));
//	}
//
//	void add_uint64(string name, uint64_t value) {
//		parameters.insert(std::make_pair(name, new UInt64Parameter(name, value)));
//	}
//
//	void add_float_int(string name, uint32_t value) {
//		parameters.insert(std::make_pair(name, new FloatIntParameter(name, value)));
//	}
//
//	void add_double_int(string name, uint64_t value) {
//		parameters.insert(std::make_pair(name, new DoubleIntParameter(name, value)));
//	}
//
//	void add_long_int(string name, long int value) {
//		parameters.insert(std::make_pair(name, new LongIntParameter(name, value)));
//	}
//
//	void add_short_int(string name, short int value) {
//		parameters.insert(std::make_pair(name, new ShortIntParameter(name, value)));
//	}
//
//	void add_long_double_int(string name, uint64_t value) {
//		parameters.insert(std::make_pair(name, new LongDoubleIntParameter(name, value)));
//	}
//
//	void add_string(string name, string value) {
//		parameters.insert(std::make_pair(name, new StringParameter(name, value)));
//	}
//
//	void add_wstring(string name, string value) {
//		parameters.insert(std::make_pair(name, new WStringParameter(name, value)));
//	}
//
//	void add_matrix_info(string name, const MatrixInfo_ptr value) {
//		matrix_info_names.push_back(name);
//		parameters.insert(std::make_pair(name, new MatrixInfoParameter(name, value)));
//	}
//
//	void add_distmatrix(string name, DistMatrix_ptr value) {
//		distmatrix_names.push_back(name);
//		parameters.insert(std::make_pair(name, new DistMatrixParameter(name, value)));
//	}
//
//	void add_ptr(string name, void * value) {
//		ptr_names.push_back(name);
//		parameters.insert(std::make_pair(name, new PointerParameter(name, value)));
//	}
//
//	int get_char(string name) const {
//		return std::dynamic_pointer_cast<CharParameter> (parameters.find(name)->second)->get_value();
//	}
//
//	signed char get_signed_char(string name) const {
//		return std::dynamic_pointer_cast<SignedCharParameter> (parameters.find(name)->second)->get_value();
//	}
//
//	unsigned char get_unsigned_char(string name) const {
//		return std::dynamic_pointer_cast<UnsignedCharParameter> (parameters.find(name)->second)->get_value();
//	}
//
//	unsigned char get_byte(string name) const {
//		return std::dynamic_pointer_cast<ByteParameter> (parameters.find(name)->second)->get_value();
//	}
//
//	bool get_bool(string name) const {
//		return std::dynamic_pointer_cast<BoolParameter> (parameters.find(name)->second)->get_value();
//	}
//
//	bool get_logical(string name) const {
//		return std::dynamic_pointer_cast<LogicalParameter> (parameters.find(name)->second)->get_value();
//	}
//
//	int get_int(string name) const {
//		return std::dynamic_pointer_cast<IntParameter> (parameters.find(name)->second)->get_value();
//	}
//
//	int8_t get_integer1(string name) const {
//		return std::dynamic_pointer_cast<Integer1Parameter> (parameters.find(name)->second)->get_value();
//	}
//
//	int16_t get_integer2(string name) const {
//		return std::dynamic_pointer_cast<Integer2Parameter> (parameters.find(name)->second)->get_value();
//	}
//
//	int32_t get_integer4(string name) const {
//		return std::dynamic_pointer_cast<Integer4Parameter> (parameters.find(name)->second)->get_value();
//	}
//
//	int64_t get_integer8(string name) const {
//		return std::dynamic_pointer_cast<Integer8Parameter> (parameters.find(name)->second)->get_value();
//	}
//
//	int8_t get_int8(string name) const {
//		return std::dynamic_pointer_cast<Int8Parameter> (parameters.find(name)->second)->get_value();
//	}
//
//	int16_t get_int16(string name) const {
//		return std::dynamic_pointer_cast<Int16Parameter> (parameters.find(name)->second)->get_value();
//	}
//
//	int32_t get_int32(string name) const {
//		return std::dynamic_pointer_cast<Int32Parameter> (parameters.find(name)->second)->get_value();
//	}
//
//	int64_t get_int64(string name) const {
//		return std::dynamic_pointer_cast<Int64Parameter> (parameters.find(name)->second)->get_value();
//	}
//
//	uint8_t get_uint8(string name) const {
//		return std::dynamic_pointer_cast<UInt8Parameter> (parameters.find(name)->second)->get_value();
//	}
//
//	uint16_t get_uint16(string name) const {
//		return std::dynamic_pointer_cast<UInt16Parameter> (parameters.find(name)->second)->get_value();
//	}
//
//	uint32_t get_uint32(string name) const {
//		return std::dynamic_pointer_cast<UInt32Parameter> (parameters.find(name)->second)->get_value();
//	}
//
//	uint64_t get_uint64(string name) const {
//		return std::dynamic_pointer_cast<UInt64Parameter> (parameters.find(name)->second)->get_value();
//	}
//
//	long get_long(string name) const {
//		return std::dynamic_pointer_cast<LongParameter> (parameters.find(name)->second)->get_value();
//	}
//
//	long long get_long_long(string name) const {
//		return std::dynamic_pointer_cast<LongLongParameter> (parameters.find(name)->second)->get_value();
//	}
//
//	unsigned get_unsigned(string name) const {
//		return std::dynamic_pointer_cast<UnsignedParameter> (parameters.find(name)->second)->get_value();
//	}
//
//	unsigned long get_unsigned_long(string name) const {
//		return std::dynamic_pointer_cast<UnsignedLongParameter> (parameters.find(name)->second)->get_value();
//	}
//
//	unsigned long long get_unsigned_long_long(string name) const {
//		return std::dynamic_pointer_cast<UnsignedLongLongParameter> (parameters.find(name)->second)->get_value();
//	}
//
//	float get_float(string name) const {
//		return std::dynamic_pointer_cast<FloatParameter> (parameters.find(name)->second)->get_value();
//	}
//
//	double get_double(string name) const {
//		return std::dynamic_pointer_cast<DoubleParameter> (parameters.find(name)->second)->get_value();
//	}
//
//	long double get_long_double(string name) const {
//		return std::dynamic_pointer_cast<LongDoubleParameter> (parameters.find(name)->second)->get_value();
//	}
//
//	string get_string(string name) const {
//		return std::dynamic_pointer_cast<StringParameter>(parameters.find(name)->second)->get_value();
//	}
//
//	MatrixInfo_ptr get_matrix_info(string name) const {
//		return std::dynamic_pointer_cast<MatrixInfoParameter>(parameters.find(name)->second)->get_value();
//	}
//
//	DistMatrix_ptr get_distmatrix(string name) const {
//		return std::dynamic_pointer_cast<DistMatrixParameter>(parameters.find(name)->second)->get_value();
//	}
//
//	void * get_ptr(string name) const {
//		return std::dynamic_pointer_cast<PointerParameter>(parameters.find(name)->second)->get_value();
//	}
//
//	string to_string() const {
//		stringstream arg_list;
//
//		for (auto it = parameters.begin(); it != parameters.end(); it++ ) {
//			arg_list << it->first << " (" << it->second->dt << "): "<< it->second->to_string() << std::endl;
//		}
//
//		return arg_list.str();
//	}
//
//private:
//	std::map<string, std::shared_ptr<Parameter> > parameters;
//};
