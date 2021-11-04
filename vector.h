class vec3_t {
public:
	vec3_t();
	vec3_t(float, float, float);
	~vec3_t();

	float x{ }, y{ }, z{ };

	void make_absolute() {
		x = std::abs(x);
		y = std::abs(y);
		z = std::abs(z);
	}

	vec3_t& operator+=(const vec3_t& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	vec3_t& operator-=(const vec3_t& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	vec3_t& operator*=(const float v) {
		x *= v;
		y *= v;
		z *= v;
		return *this;
	}

	vec3_t operator+(const vec3_t& v) {
		return vec3_t{ x + v.x, y + v.y, z + v.z };
	}

	vec3_t operator-(const vec3_t& v) {
		return vec3_t{ x - v.x, y - v.y, z - v.z };
	}

	vec3_t& operator+(const float& v) {
		x = x + v;
		y = y + v;
		z = z + v;
		return *this;
	}

	vec3_t& operator-(const float& v) {
		x = x - v;
		y = y - v;
		z = z - v;
		return *this;
	}

	vec3_t operator*(const float v) {
		return vec3_t{ x * v, y * v, z * v };
	}

	vec3_t operator/(const float fl) const {
		return vec3_t(x / fl, y / fl, z / fl);
	}

	vec3_t operator+(const vec3_t& v) const {
		return vec3_t(x + v.x, y + v.y, z + v.z);
	}

	bool operator==(const vec3_t& v) const {
		return x == v.x && y == v.y && z == v.z;
	}

	bool operator!=(const vec3_t& v) const {
		return x != v.x || y != v.y || z != v.z;
	}

	vec3_t operator*(const float fl) const {
		return vec3_t(x * fl, y * fl, z * fl);
	}

	vec3_t operator*(const vec3_t& v) const {
		return vec3_t(x * v.x, y * v.y, z * v.z);
	}

	vec3_t operator-(const vec3_t& v) const {
		return vec3_t(x - v.x, y - v.y, z - v.z);
	}

	vec3_t& operator/=(const float fl) {
		x /= fl;
		y /= fl;
		z /= fl;
		return *this;
	}

	float operator[](const int i) const {

		return ((float*)this)[i];
		// ReSharper restore CppCStyleCast
	}

	float& operator[](const int i) {
		return reinterpret_cast<float*>(this)[i];
	}

	[[nodiscard]] float length() const {
		return sqrt(x * x + y * y + z * z);
	}

	[[nodiscard]] float length_sqr() const {
		return x * x + y * y + z * z;
	}

	[[nodiscard]] float length_2d() const {
		return sqrt(x * x + y * y);
	}

	[[nodiscard]] float dist_to(const vec3_t& other) const {
		vec3_t delta;

		delta.x = x - other.x;
		delta.y = y - other.y;
		delta.z = z - other.z;

		return delta.length();
	}

	void clamp() {
		if (x > 75.f) x = 75.f;
		else if (x < -75.f) x = -75.f;
		if (z < -180) z += 360.0f;
		else if (z > 180) z -= 360.0f;

		y = 0.f;
	}

	inline double Length() {
		return sqrt(x * x + y * y + z * z);
	}

	void normalize() {
		while (this->x > 89.0f)
			this->x -= 180.0f;

		while (this->x < -89.0f)
			this->x += 180.0f;

		while (this->y > 180.0f)
			this->y -= 360.0f;

		while (this->y < -180.0f)
			this->y += 360.0f;
	}


	[[nodiscard]] float dot(const vec3_t& other) const {
		return x * other.x + y * other.y + z * other.z;
	}

	[[nodiscard]] bool empty() const {
		return x == 0 || y == 0 || z == 0;
	}

	void init(const float ix = 0.0f, const float iy = 0.0f, const float iz = 0.0f) {
		x = ix;
		y = iy;
		z = iz;
	}
};

vec3_t::vec3_t() {
	x = y = z = 0.0f;
}

vec3_t::vec3_t(const float fx, const float fy, const float fz) {
	x = fx;
	y = fy;
	z = fz;
}

vec3_t::~vec3_t() = default;

inline float sqrtf_(float x)
{
	union { float f; uint32_t i; } z = { x };
	z.i = 0x5f3759df - (z.i >> 1);
	z.f *= (1.5f - (x * 0.5f * z.f * z.f));
	z.i = 0x7EEEEEEE - z.i;
	return z.f;
}
bool IF_AIMKEY()
{
	return GetAsyncKeyState(VK_RBUTTON);
}
double powf_(double x, int y)
{
	double temp;
	if (y == 0)
		return 1;
	temp = powf_(x, y / 2);
	if ((y % 2) == 0) {
		return temp * temp;
	}
	else {
		if (y > 0)
			return x * temp * temp;
		else
			return (temp * temp) / x;
	}
}