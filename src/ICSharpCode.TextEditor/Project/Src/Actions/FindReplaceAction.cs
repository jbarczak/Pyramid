using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ICSharpCode.TextEditor.Actions
{
    public class FindReplaceAction : AbstractEditAction
    {
        public override void Execute(TextArea textArea)
        {
            ICSharpCode.TextEditor.FindAndReplaceForm frm = new FindAndReplaceForm();
            frm.ShowFor(textArea.MotherTextEditorControl, false);
        }
    }
}
