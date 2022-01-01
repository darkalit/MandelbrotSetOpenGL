#ifndef COMPLEXN_H
#define COMPLEXN_H

#include <iostream>
#include <string>

namespace cmpx
{
	float PI = atan(1) * 4;

	class Complexn
	{
	private:
		float re, im;
		float round(float num) {
			return std::roundf(num * 100000) / 100000;
		}
	public:
		Complexn() { re = im = 0; }
		Complexn(float a, float b) :
			re(round(a)), im(round(b))
		{}

		friend std::ostream& operator<<(std::ostream& out, const Complexn& cnm)
		{
			if (cnm.re == 0 && cnm.im == 1)
				out << "i";
			else if (cnm.re == 0 && cnm.im == -1)
				out << "-i";
			else if (cnm.re == 0)
				out << cnm.im << "i";
			else if (cnm.im == 0)
				out << cnm.re;
			else if (cnm.im > 0)
				out << cnm.re << "+" << cnm.im << "i";
			else if (cnm.im < 0)
				out << cnm.re << cnm.im << "i";

			return out;
		}

		Complexn operator+(Complexn r)
		{
			return Complexn(re + r.re, im + r.im);
		}
		friend Complexn operator+(float l, Complexn r)
		{
			return Complexn(l + r.re, r.im);
		}
		friend Complexn operator+(Complexn l, float r)
		{
			return Complexn(r + l.re, l.im);
		}

		Complexn operator-(Complexn r)
		{
			return Complexn(re - r.re, im - r.im);
		}
		friend Complexn operator-(float l, Complexn r)
		{
			return Complexn(l - r.re, -r.im);
		}
		friend Complexn operator-(Complexn l, float r)
		{
			return Complexn(l.re - r, l.im);
		}

		Complexn operator*(Complexn r)
		{
			return Complexn(re * r.re - im * r.im, re * r.im + im * r.re);
		}
		friend Complexn operator*(float l, Complexn r)
		{
			return Complexn(l * r.re, l * r.im);
		}
		friend Complexn operator*(Complexn l, float r)
		{
			return Complexn(l.re * r, l.im * r);
		}

		Complexn operator/(Complexn r)
		{
			return Complexn(
				(re * r.re + im * r.im) / (r.re * r.re + r.im * r.im),
				(im * r.re - re * r.im) / (r.re * r.re + r.im * r.im));
		}
		friend Complexn operator/(float l, Complexn r)
		{
			return Complexn(
				(l * r.re) / (r.re * r.re + r.im * r.im),
				(-l * r.im) / (r.re * r.re + r.im * r.im));
		}
		friend Complexn operator/(Complexn l, float r)
		{
			return Complexn(
				l.re / r, l.im / r);
		}

		Complexn& operator=(const Complexn& cnm)
		{
			re = cnm.re;
			im = cnm.im;
			return *this;
		}

		friend bool operator== (const Complexn& l, const Complexn& r)
		{
			return (l.re == r.re) && (l.im == r.im);
		}
		friend bool operator== (const Complexn& l, const float& r)
		{
			return (l.re == r);
		}
		friend bool operator== (const float& l, const Complexn& r)
		{
			return (l == r.re);
		}

		friend bool operator!= (const Complexn& l, const Complexn& r)
		{
			return (l.re != r.re) && (l.im != r.im);
		}
		friend bool operator!= (const Complexn& l, const float& r)
		{
			return (l.re != r);
		}
		friend bool operator!= (const float& l, const Complexn& r)
		{
			return (l != r.re);
		}

		float Re()
		{
			return re;
		}

		float Im()
		{
			return im;
		}

		float aRad() // Angle in radians
		{
			return atan2(im, re);
		}

		int aDeg() // Angle in degrees
		{
			return aRad() * 180 / PI;
		}

		float norm()
		{
			return re * re + im * im;
		}

		float abs()
		{
			return std::sqrt(norm());
		}

		std::string exp()
		{
			return std::to_string(abs()) + " * exp(" + std::to_string(aRad()) + "i)";
		}
	};

	Complexn i(0, 1);

	Complexn conj(Complexn num)
	{
		return Complexn(num.Re(), -num.Im());
	}

	Complexn polar(float r, float theta)
	{
		return Complexn(r * std::cos(theta), r * std::sin(theta));
	}

	Complexn exp(Complexn num)
	{
		return polar(std::exp(num.Re()), num.Im());
	}

	Complexn log(Complexn num)
	{
		return std::log(num.abs()) + i * num.aRad();
	}

	Complexn log10(Complexn num)
	{
		return log(num) / std::log(10);
	}

	Complexn pow(Complexn num, int power)
	{
		Complexn operand(num.Re(), num.Im());
		if (power == 0)
			return Complexn(1, 0);
		if (power == 1)
			return operand;
		Complexn temp(1, 0);
		for (int i = 0; i < power; ++i)
			temp = temp * operand;
		return temp;
	}

	Complexn pow(Complexn num, float power, int k = 0)
	{
		return std::pow(num.abs(), power) * polar(1, (power) * (num.aRad() + 2 * PI * k));
	}

	Complexn sqrt(Complexn num, int k = 0)
	{
		return std::sqrt(num.abs()) * polar(1, (1.f / 2) * (num.aRad() + 2 * PI * k));
	}

	Complexn cbrt(Complexn num, int k = 0)
	{
		return std::cbrt(num.abs()) * polar(1, (1.f / 3) * (num.aRad() + 2 * PI * k));
	}

	Complexn sin(Complexn num)
	{
		return Complexn(
			std::sin(num.Re()) * std::cosh(num.Re()), 
			std::cos(num.Im()) * std::sinh(num.Im()));
	}

	Complexn sinh(Complexn num)
	{
		return Complexn(
			std::sinh(num.Re()) * std::cos(num.Im()),
			std::cosh(num.Re()) * std::sin(num.Im()));
	}

	Complexn cos(Complexn num)
	{
		return Complexn(
			 std::cos(num.Re()) * std::cosh(num.Im()), 
			-std::sin(num.Re()) * std::sinh(num.Im()));
	}

	Complexn cosh(Complexn num)
	{
		return Complexn(
			std::cosh(num.Re()) * std::cos(num.Im()),
			std::sinh(num.Re()) * std::sin(num.Im()));
	}

	Complexn tan(Complexn num)
	{
		return sin(num) / cos(num);
	}

	Complexn tanh(Complexn num)
	{
		return sinh(num) / cosh(num);
	}

	Complexn ctg(Complexn num)
	{
		return cos(num) / sin(num);
	}

	Complexn ctgh(Complexn num)
	{
		return cosh(num) / sinh(num);
	}

	Complexn cis(float num)
	{
		return Complexn(std::cos(num), std::sin(num));
	}
}
#endif // COMPLEXN_H