#include "Sequencer.h"
//using namespace std;

// NOTE: "++i" is more efficieht than "i++" inside for loops.

Sequencer::Sequencer() {
	ofLogVerbose(" sequencer ") << "contructed!";
}

Sequencer::~Sequencer() {
	ofLogVerbose(" sequencer ") << "destroyed!";
}

void Sequencer::setup(OscManager * _oscMan) {
	ofLogVerbose(" sequencer ") << "setup: table initialization!";

  // obtain a reference to the OscManager
  // we will use later to play our sounds!
  oscMan = _oscMan;

	for (int column=0; column<COLS; column++)
	{
		for (int row=0; row<ROWS; row++)
		{
			for (int plane=0; plane<PLANES; plane++)
			{
				table[row][column][plane]=0;
			}
		}
	}
  //print();
}

void Sequencer::tuioAdd(int plane, int column, int sess_id)
{
  // check if plane is a valid one
  if( plane<1 || plane >3 )
  {
    ofLogError(" sequencer ") << "tuioAdd: Invalid Plane! ("<< plane <<")";
    return;
  }
  // check if the column is a valid one
  if( column<0 || column >7 )
  {
    ofLogError(" sequencer ") << "tuioAdd: Invalid column! ("<< column <<")";
    return;
  }

  // As soon as I find a place where to save
  // the session id inside the table,
  // I'll use it to place the associated sound
  // inside the corresponding plane
	int row;

  bool found_space = false;

	for (row=0; row<ROWS; row++)
	{
		if ( table[row][column][0] == 0 )
		{
			table[row][column][0] = sess_id;
      found_space = true;
			break;
		}
	}

  // the particular column where we want to store
  // the session ids is already full of numbers!!
  // we have no more space to add new session_ids
  // TODO: fix in case there's no more space
  if( !found_space )
  {
    ofLogWarning(" sequencer ") << "tuioAdd: No more space! in column " << column
					<< ":"<< " unable to store the new session id: " << sess_id << ";";
    return;
  }

	for (int p=1; p<PLANES; p++) //++p ore p++
	{
		if (p == plane)
    {
			table[row][column][p]=1;
      return;
    }
		//else
		//	table[row][column][p]=0; // TODO: maybe to be removed (not sure about that)!!
	}

	//print(0);
}

void Sequencer::tuioUpdate(int plane, int column, int sess_id)
{
	//ofLogVerbose(" sequencer ") << "tuioUpdate" << " (id: "<<  sess_id << ");";

	// maybe calling those methods is
	// unefficient in some way?
	tuioRemove(plane, sess_id);
	tuioAdd(plane, column, sess_id);
}

void Sequencer::tuioRemove(int sess_id)
{
  // search for the session id inside the
  // first plane
  for (int column=0; column<COLS; column++)
  {
    for (int row=0;row<ROWS;row++)
    {
      if (table[row][column][0] == sess_id)
			{
        // we have found the session id
        // now we look for the corresponding sound
        for(int p=1; p<PLANES; p++)
        {
          if( table[row][column][p] == 1 )
          {
            // we have found the corresponding sound inside the table
            // so now we re-initialize those table cells
            table[row][column][0] = 0;
            table[row][column][p] = 0;
            return;
          }
        }
      }
    }
  }
  ofLogWarning(" sequencer ") << "tuioRemove (sess_id): session id not found"
															<< " (id: "<<  sess_id << ");";
}

void Sequencer::tuioRemove(int plane, int sess_id)
{
  // check if plane is a valid one
  if( plane<1 || plane >3 )
  {
    ofLogError(" sequencer ")  << "tuioRemove (plane, sess_id): Invalid Plane!";
    return;
  }

	for (int column=0; column<COLS; column++)
	{
		for (int row=0;row<ROWS;row++)
		{
			if (table[row][column][0] == sess_id)
			{
				table[row][column][0]=0;
				table[row][column][plane]=0;
        return; // we can return as soon as we find our boy
			}
		}
	}
  ofLogWarning(" sequencer ")  << "tuioRemove (plane, sess_id): session id not found"
															<< " (id: "<<  sess_id << "; plane: " << plane << ");";
	//print(0);
}

// this function is called by the timing thread and is triggered
// every new 8th note (which number is passed as a parameter).
void Sequencer::play(int & column)
{
  //cout << "SEQUENCER PLAY: now playing column " << column << endl;
  for(int p=1; p<PLANES; p++)
  {
    //cout << "\tSEQUENCER PLAY: now examining plane " << p << endl;
    for(int r=0; r<ROWS; r++)
    {
      //cout << "\t\tSEQUENCER PLAY: now examining row " << r << endl;
      if( table[r][column][p] == 1)
      {
        // we've found a sound here so we play it
        oscMan->send("/instrument", p);
        // now it doesn't matter if there are others sound,
        // they will be of the very same type so triggering
        // only one of them is sufficien: let's examine
        // the next instrument/plane.
        break;
      }
      else
      {
        //cout << "\t\tSEQUENCER PLAY: nothing to play here" << endl;
      }
    }
  }
}

void Sequencer::print()
{
  for(int p=0; p<PLANES; p++)
    print(p);
}

void Sequencer::print(int plane)
{
  ofLogVerbose(" sequencer ")  << "print: plane " << plane << ": [";
	//int row, column;
	for(int row=0; row<ROWS; row++)
	{
		for (int column=0; column<COLS; column++)
		{
			cout << table[row][column][plane] << " ";
		}
		cout << endl;
	}
  ofLogVerbose(" sequencer ") << "]\n";
}
