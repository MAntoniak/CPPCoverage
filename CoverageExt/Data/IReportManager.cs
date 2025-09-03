﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NubiloSoft.CoverageExt.Data
{
    public interface IReportManager
    {
        ICoverageData UpdateData();
        void ResetData();
        bool IsValid(Settings instance);
    }
}
