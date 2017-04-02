class Item
{
  public:
    virtual void Use(/* creature* target*/) = 0,
                 Equip(/* creature* target*/) = 0,
                 unEquip(/* creature* target*/) = 0;
    virtual Item(int grade,int identifier),
            ~Item();
};

class Potion : public Item
{
  
};

class Scroll : public Item
{
  
};

class Weapon : public Item
{
  
};

class Armor : public Item
{
  
};
