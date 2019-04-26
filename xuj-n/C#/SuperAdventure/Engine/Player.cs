using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Engine
{
    public class Player : LivingCreature
    {
        public Player(int currentHitPt, int maximumHitPt, int gold, int experiencePt, int level)
            :base(currentHitPt, maximumHitPt)
        {
            Gold = gold;
            ExperiencePoints = experiencePt;
            Level = level;
            Inventory = new List<InventoryItem>();
            Quests = new List<PlayerQuest>();
        }
        public int Gold { get; set; }
        public int ExperiencePoints { get; set; }
        public int Level { get; set; }
        public List<InventoryItem> Inventory { get; set; }
        public List<PlayerQuest> Quests { get; set; }
        public Location CurrentLocation { get; set; }

    }
}
