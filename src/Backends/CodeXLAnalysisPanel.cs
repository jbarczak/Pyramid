using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Pyramid
{
    public partial class CodeXLAnalysisPanel : UserControl
    {
        private Dictionary<string, List<KeyValuePair<string,string>>> m_DataSet = new Dictionary<string, List<KeyValuePair<string,string>>>();

        public CodeXLAnalysisPanel()
        {
            InitializeComponent();
        }

        public void AddResult(string asic, Dictionary<string, string> vals)
        {
            List<KeyValuePair<string,string>> items = new List<KeyValuePair<string,string>>();

            items.Add(new KeyValuePair<string,string>("Asic", asic));
            items.AddRange(vals);
            m_DataSet.Add(asic, items);
            if (m_DataSet.Count == 1)
                PopulateListView(items);
        }

        public void SetAsic(string asic)
        {
            listView1.Items.Clear();
            List<KeyValuePair<string,string>> items;
            if (m_DataSet.TryGetValue(asic, out items))
                PopulateListView(items);
        }

        private void PopulateListView(List<KeyValuePair<string, string>> items)
        {
            foreach (KeyValuePair<string, string> p in items)
            {
                string[] arr = new string[2]; //ahoy!
                arr[0] = p.Key;
                arr[1] = p.Value;
                listView1.Items.Add(new ListViewItem(arr));
            }
        }
    }
}
