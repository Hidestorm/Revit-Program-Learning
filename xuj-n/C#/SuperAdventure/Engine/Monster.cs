using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Engine
{
    public class Monster : LivingCreature
    {
        public Monster(int id, string name, int maximumD, int rewardExpPt, int rewardGold, int currentHitPt, int maximumHitPt)
            :base(currentHitPt, maximumHitPt)
        {
            ID = id;
            Name = name;
            MaximumDamage = maximumD;
            RewardExperiencePoints = rewardExpPt;
            RewardGold = rewardGold;
            LootTable = new List<LootItem>();
        }
        public int ID { get; set; }
        public string Name { get; set; }
        public int MaximumDamage { get; set; }
        public int RewardExperiencePoints { get; set; }
        public int RewardGold { get; set; }
        public List<LootItem> LootTable { get; set; }
    }
}
