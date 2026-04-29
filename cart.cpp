#include <Shoppingheader.h>
class Product
{
public:
	int id;
	string name;
	double price;
public:
	Product()
	{
		id = 0;
		name = "";
		price = 0;
	}
	Product(int i, string n, double p)
	{
		id = i;
		name = n;
		price = p;
	}
	void Display() const
	{
		cout << id << "|" << name << "|" << price << "|" << endl;
	}
	string getname() const
	{
		return name;
	}
	double getprice()
	{
		return price;
	}
	bool operator==(const Product& obj) const
	{
		if (this->id == obj.id)
		{
			return true;
		}
		else 
		{
			return false;
		}
	}
	void print()
	{
		cout << " Product Name : " << name << endl;
		cout << " Price : " << price << endl;
	}
};
class Cartitem {
public:
	Product item;
	unsigned long int quantity;
public:
	Cartitem()
	{
		quantity = 0;
	}
	Product getitem()
	{
		return item;
	}
	Cartitem& operator--()
	{
		if (quantity > 0)
		{
			quantity--;
		}
		return *this;
	}
};
class Cart {
	unsigned long long int Cid;
	Cartitem* p;
	int count;
public:
	Cart()
	{
		Cid = 0;
		p = nullptr;
		count = 0;
	}
	Cart(unsigned long long int Ci)
	{
		Cid = Ci;
		p = nullptr;
		count = 0;
	}
	void addproduct(Product pro,int q)
	{
		if (count == 0)
		{
			p = new Cartitem[1];
			p[0].item = pro;
			p[0].quantity = q;
			count = 1;
			return;
		}
		
		for (int i = 0; i < count; i++)
		{
			if (p[i].item == pro)
			{
				p[i].quantity += q;
				return;
			}
		}
		Cartitem* temp = new Cartitem[count + 1];
		for (int i = 0; i < count; i++)
		{
			temp[i] = p[i];
		}
		delete[] p;
		temp[count].item = pro;
		temp[count].quantity = q;
		count++;
		p = temp;
	}
	void removeproduct(Product pro)
	{
		int index = -1;
		for (int i = 0; i < count; i++)
		{
			if (p[i].item == pro)
			{
				index = i;
				break;
			}
		}

		if (index == -1)
		{
			cout << " Product not found " << endl;
			return;
		}

		Cartitem* temp = new Cartitem[count - 1];

		int j = 0;
		for (int i = 0; i < count; i++)
		{
			if (i != index)
			{
				temp[j] = p[i];
				j++;
			}
		}

		delete[] p;

		p = temp;
		count--;
	}
	void viewcart()
	{
		for (int i = 0; i < count; i++)
		{
			p[i].item.print();
			cout << " Quantity :  " << p[i].quantity << endl;
		}
	}
	void upgradequnatity(int productid, int newquantity)
	{
		for (int i = 0; i < count; i++)
		{
			if (p[i].item.id == productid)
			{
				if (newquantity < 0) {
					cout << "Error! Invalid quantity." << endl;
					return;
				}

				if (newquantity == 0)
				{
					removeproduct(p[i].item);
					return;
				}
				else
				{
					p[i].quantity = newquantity;
					return;
				}
			}
		}
		cout << "Product not found in cart " << endl;
	}
	long double calculatetotal()
	{
		long double total = 0;

		for (int i = 0; i < count; i++)
		{
			total += (p[i].item.getprice() * p[i].quantity);
		}

		return total;
	}
	void clearcart()
	{
		delete[] p;
		p = nullptr;
		count = 0;
	}
	~Cart()
	{
		delete[] p;
		p = nullptr;
	}
};

