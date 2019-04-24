using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Engine
{
    public class LivingCreature
    {
        public LivingCreature(int currentHitPt, int maximumHitPt)
        {
            CurrentHitPoints = currentHitPt;
            MaximumHitPoints = maximumHitPt;
        }
        public int CurrentHitPoints { set; get; }
        public int MaximumHitPoints { set; get; }
    }
}
