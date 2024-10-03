#include <iostream>
#include <memory>
#include <string>
#include <vector>

std::string vColor[] = { "Red","Green","Blue" };
enum class Color { Red, Green, Blue } ;

/**
 * \brief Globale variable for print function
 */
std::string vSize[] = { "Small","Medium","Large" };
enum class Size { Small, Medium, Large};


struct Product
{
	std::string		m_Name;
	Color			m_Color;
	Size			m_Size;
};

typedef std::vector<Product*> Items;

template <typename T>
struct ISpecification
{
	virtual bool is_specification(T* item) = 0;
};

struct ColorSpecification : public ISpecification<Product>
{
	Color color;

	explicit ColorSpecification(Color color)
		: color(color)
	{
	}

	bool is_specification(Product* s) override
	{
		return (s->m_Color == color);
	}
};

struct SizeSpecification : public ISpecification<Product>
{
	Size size;

	explicit SizeSpecification(Size size)
		: size(size)
	{
	}

	bool is_specification(Product* s) override
	{
		return (s->m_Size == size);
	}
};


struct SizeAndColorSpecification : public ISpecification<Product>
{
	SizeSpecification size;
	ColorSpecification color;

	SizeAndColorSpecification( SizeSpecification& size,  ColorSpecification& color)
		: size(size),
		  color(color)
	{
	}


	bool is_specification(Product* s) override
	{
		return (size.is_specification(s) && color.is_specification(s));
	}
};


template <typename T>
struct IFind
{
	virtual std::vector<T*>Find(std::vector<T*> items, ISpecification<T>& spec) = 0;
};

struct FindProduct : public IFind<Product>
{
	virtual std::vector<Product*>Find(std::vector<Product*> items, ISpecification<Product>& spec)
	{
		Items results;
		for (auto& p : items)
			if (spec.is_specification(p))
				results.push_back(p);

		return  results;
	}
};

int main(int argc, char ** argv)
{

    Product apple{ "Apple", Color:: Green, Size:: Small};
    Product car{ "Car", Color:: Blue, Size::Large};
    Product tree{ "Tree", Color:: Green, Size::Large};
    ColorSpecification green{Color::Green};
    SizeSpecification large{Size::Large};
    SizeAndColorSpecification size_color{large , green};

    FindProduct fp;
    std:: vector<Product*> allproducts {&apple,&tree,&car};

    auto results=fp.Find(allproducts, size_color);

    for (auto& i : results)
    {
        std:: cout<< i->m_Name << " : is "<< vColor[(int)i->m_Color].c_str()<< std:: endl;
    }
    return 0;
}