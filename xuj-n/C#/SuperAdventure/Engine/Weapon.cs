using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Engine
{
    public class Weapon : Item
    {
        public Weapon(int id, string name, string namePlural, int minimumD, int maximumD):
            base(id, name, namePlural)
        {
            MinimumDamage = minimumD;
            MaximumDamage = maximumD;
        }
        public int MinimumDamage { get; set; }
        public int MaximumDamage { get; set; }
    }
}
