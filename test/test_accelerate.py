import os
import unittest

from accelerate import Dataset, Workspace

class TestAccelerateDataset(unittest.TestCase):

    def test_read(self):
        in_table = '{0}/ACLED_Sample'.format(os.environ['accelerate_sample_gdb'])
        dataset = Dataset()
        dataset.read(in_table, ['OID@', 'event_type'], '1=1')
        dataframe = dataset.to_pandas()
        row_count = dataframe.count()[0]
        self.assertLess(0, row_count, 'The dataframe should not be empty!')
        head = dataframe.head()
        self.assertIsNotNone(head, 'The first rows must not be none!')
        columns = dataframe.columns
        self.assertIsNotNone(columns, 'The columns must not be none!')
        self.assertEqual(2, len(columns), 'Two columns were expected!')

    def test_write(self):
        in_table = '{0}/ACLED_Sample'.format(os.environ['accelerate_sample_gdb'])
        dataset = Dataset()
        dataset.read(in_table, ['OID@', 'event_type'], '1=1')
        
        out_path = os.environ['accelerate_sample_gdb']
        out_table = 'ACLED_Empty'
        dataset.write(out_path, out_table, ['event_type'])


class TestAccelerateWorkspace(unittest.TestCase):

    def test_create_table(self):
        out_path = os.environ['accelerate_sample_gdb']
        out_table = 'Table_New'
        workspace = Workspace(out_path)
        table = workspace.create_table(out_table, True)
        self.assertIsNotNone(table, 'The table must not be none!')



if __name__ == '__main__':
    unittest.main()