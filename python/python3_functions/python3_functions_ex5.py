def product_sales(my_dict):
    ten_percent_sale = {k: v * 0.9 for k, v in my_dict.items()}
    return ten_percent_sale

product_dict = {'iPad': 700, 'iPhone': 800, 'MacBook': 1099}
sales_price = product_sales(product_dict)
print(sales_price)