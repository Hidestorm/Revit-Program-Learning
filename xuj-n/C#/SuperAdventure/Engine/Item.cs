using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Engine
{
    public class Item
    {
        public Item (int id, string name, string namePlural)
        {
            ID = id;
            this.Name = name;
            this.NamePlural = namePlural;
        }
        public int ID { set; get; }
        public string Name { get; set; }
        public string NamePlural { get; set; }
    }
}
