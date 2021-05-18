import os
import unittest

from accelerate import Dataset

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
        
        out_table = '{0}/ACLED_Empty'.format(os.environ['accelerate_sample_gdb'])
        dataset.write(out_table, ['OID@', 'event_type'])


if __name__ == '__main__':
    unittest.main()