package Project2;

import java.awt.*;
import java.awt.event.*;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;

import javax.swing.*;

class SettingPanel extends JPanel 
{
	Calendar win;
	
	public SettingPanel(Calendar win)
	{
		setLayout(new FlowLayout(1,10,65));
		this.win = win;
		
		List Yearlist = new List(8, false);
		List Monthlist = new List(8, false);
			
		for (int i = 1950; i <= 2020; i++)								
			Yearlist.add(Integer.toString(i));
			
		for (int i = 1; i <= 12; i++)
			Monthlist.add(Integer.toString(i));
		
		JLabel year = new JLabel("년도");
		JLabel month = new JLabel("달");
		JButton Enter = new JButton("확인");
		
		add(year);
		add(Yearlist);
		add(month);
		add(Monthlist);
		add(Enter);
		
		Enter.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e)
			{
				int yearindex = Yearlist.getSelectedIndex();
				int monthindex = Monthlist.getSelectedIndex();
			
				if (yearindex != -1 && monthindex != -1)
				{				
					int Year = Integer.parseInt(Yearlist.getItem(yearindex));
					int Month = Integer.parseInt(Monthlist.getItem(monthindex));									
					
					win.Change("Calendar", Year, Month, "");
				}
				else
					System.out.println("년도와 월을 모두 선택하세요.");
			}
		});		
	}
}

class CalendarPanel extends JPanel
{
	Calendar win;
	int Year, Month;
	String day;
	
	public void SetYearMonth(int year, int month)
	{
		Year = year;
		Month = month;
	}
	
	public void SettingCalendarPanel(Calendar _win) 
	{
		GridLayout grid = new GridLayout(0,7);
		grid.setVgap(10);

		setLayout(grid);
		this.win = _win;
		
		int lastday = 0;
		int MonthArray[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
		
		if (Year % 4 == 0 && (Year % 100 != 0 || Year % 400 == 0)) MonthArray[1] = 29;
		else MonthArray[1] = 28;
		
		Year--;
		lastday = 365 * (Year - 1950) + Year / 4 - Year / 100 + Year / 400;
		
		for (int i = 0; i < Month - 1; i++) lastday += MonthArray[i];
		lastday = (lastday + 5) % 7;
		
		JLabel year = new JLabel(" " + (Year + 1) + " 년");
		JLabel month = new JLabel(Month + " 월");
		
		add(year);
		add(month);
		
		for (int i = 0; i < 4; i++) 
		{
			JLabel Empty = new JLabel();
			Empty.setLocation(i * 2, i * 2);
			Empty.setSize(3, 2);
			
			add(Empty);
		}
			
		JButton Back = new JButton("이전");
		add(Back);
		
		Back.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e)
			{				
				win.Change("Setting", Year, Month, null);
			}
		});		
		
		
		JLabel Day = new JLabel("         " + "일");
		add(Day);
		Day = new JLabel("         " + "월");
		add(Day);
		Day = new JLabel("         " + "화");
		add(Day);
		Day = new JLabel("         " + "수");
		add(Day);
		Day = new JLabel("         " + "목");
		add(Day);
		Day = new JLabel("         " + "금");
		add(Day);
		Day = new JLabel("         " + "토");
		add(Day);		
				
		for (int i = 0; i < lastday; i++)
		{
			JLabel Empty = new JLabel();
			Empty.setLocation(i * 2, i * 2);
			Empty.setSize(3, 2);
			
			add(Empty);
		}
		
		for (int i = 1; i <= MonthArray[Month - 1]; i++)
		{
			JButton b = new JButton(Integer.toString(i));
			b.setLocation(i * 2, i * 2);
			b.setSize(3, 2);
			
			if (win.YearCalenderMap.get(Integer.toString(Year + 1)).get(Integer.toString(Month)).containsKey(b.getText()))
			{				
				if(!win.YearCalenderMap.get(Integer.toString(Year + 1)).get(Integer.toString(Month)).get(b.getText()).isEmpty())
				{
					b.setBackground(new Color(0, 150, 255));
				}
			}	
			
			b.addActionListener(new CalendarAction());
			add(b);
		}
	}
	
	class CalendarAction implements ActionListener
	{
		@Override
		public void actionPerformed(ActionEvent e) {
			// TODO Auto-generated method stub
			JButton b = (JButton)e.getSource();
			
			if(win.YearCalenderMap.containsKey(Integer.toString(Year + 1)))
			{
				if (win.YearCalenderMap.get(Integer.toString(Year + 1)).containsKey(Integer.toString(Month)))
				{
					day = b.getText();
					System.out.println(day + " Day Button Push");
					
					win.Change("DayCalendar", Year + 1, Month, day);
				}
			}
		}
	}
}

class DayCalendarPanel extends JPanel
{
	Calendar win;
	int Year, Month;
	String day;
	
	public void SetYearMonthDay(int year, int month, String day)
	{
		this.Year = year;
		this.Month = month;
		this.day = day;
	}
	
	public void SettingDayCalendarPanel(Calendar _win)
	{
		this.win = _win;
		setLayout(null);
		
		JLabel Info = new JLabel(Integer.toString(Year) + " 년 " + Integer.toString(Month) + " 월 " + day + " 일 일정");
		Info.setBounds(190, 10, 300, 15);
		add(Info);
		
		List DayCalendarList = new List(15, false);
		
		if (win.YearCalenderMap.get(Integer.toString(Year)).get(Integer.toString(Month)).containsKey(day))
		{	
			LinkedList list = win.YearCalenderMap.get(Integer.toString(Year)).get(Integer.toString(Month)).get(day);			
			for (int i = 0; i < list.size(); i++)
			{
				DayCalendarList.add(list.get(i).toString());
			}				
		}
		
		DayCalendarList.setBounds(12, 30, 460, 150);
		add(DayCalendarList);
		
		JTextField TextField = new JTextField(30);
		TextField.setBounds(12, 190, 460, 20);
		add(TextField);	
		
		JButton Insert = new JButton("추가");
		Insert.setBounds(12, 220, 70, 30);
		add(Insert);
		
		Insert.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e)
			{				
				if (!TextField.getText().isEmpty())
				{
					if (win.YearCalenderMap.get(Integer.toString(Year)).get(Integer.toString(Month)).containsKey(day))
					{	
						LinkedList list = win.YearCalenderMap.get(Integer.toString(Year)).get(Integer.toString(Month)).get(day);			
						list.add(TextField.getText());
						
						DayCalendarList.add(TextField.getText());
						TextField.setText(null);
					}
				}
			}
		});		
		
		JButton Edit = new JButton("편집");
		Edit.setBounds(102, 220, 70, 30);
		add(Edit);
		
		Edit.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e)
			{				
				if (!TextField.getText().isEmpty())
				{
					if (win.YearCalenderMap.get(Integer.toString(Year)).get(Integer.toString(Month)).containsKey(day))
					{	
						if (DayCalendarList.getSelectedIndex() != -1)
						{
							LinkedList list = win.YearCalenderMap.get(Integer.toString(Year)).get(Integer.toString(Month)).get(day);			
							list.set(DayCalendarList.getSelectedIndex(), TextField.getText());
							
							DayCalendarList.replaceItem(TextField.getText(), DayCalendarList.getSelectedIndex());
							TextField.setText(null);
						}											
					}
				}
			}
		});		
		
		JButton Remove = new JButton("삭제");
		Remove.setBounds(192, 220, 70, 30);
		add(Remove);
		
		Remove.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e)
			{				
				
				if (win.YearCalenderMap.get(Integer.toString(Year)).get(Integer.toString(Month)).containsKey(day))
				{	
					if (DayCalendarList.getSelectedIndex() != -1)
					{
						LinkedList list = win.YearCalenderMap.get(Integer.toString(Year)).get(Integer.toString(Month)).get(day);			
						list.remove(DayCalendarList.getSelectedIndex());
							
						DayCalendarList.remove(DayCalendarList.getSelectedIndex());							
					}											
				}				
			}
		});		
		
		JButton Back = new JButton("이전");
		Back.setBounds(282, 220, 70, 30);
		add(Back);
		
		Back.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e)
			{				
				win.Change("Calendar", Year, Month, "");
			}
		});		
	}
}

public class Calendar extends JFrame 
{	
	private SettingPanel s = null;
	private CalendarPanel c = null;
	private DayCalendarPanel d = null;
		
	private Map<String, LinkedList<String>> DayCalendarMap = null;
	private Map<String, Map<String, LinkedList<String>>> MonthCalendarMap = null;
	public Map<String, Map<String, Map<String, LinkedList<String>>>> YearCalenderMap = new HashMap<>();
	
	public void Change(String name, int year, int month, String day)
	{
		if (name == "Calendar")
		{
			c = new CalendarPanel();
			s = null;
			
			c.SetYearMonth(year, month);
			c.SettingCalendarPanel(this);
			
			getContentPane().removeAll();
			getContentPane().add(c);
			revalidate();
			repaint();
		}
		else if (name == "Setting")
		{
			c = null;
			s = new SettingPanel(this);
			
			getContentPane().removeAll();
			getContentPane().add(s);
			revalidate();
			repaint();
		}
		
		else if (name == "DayCalendar")
		{
			c = null;
			d = new DayCalendarPanel();
			
			d.SetYearMonthDay(year, month, day);
			d.SettingDayCalendarPanel(this);
			
			getContentPane().removeAll();
			getContentPane().add(d);
			revalidate();
			repaint();
		}
	}
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		var data = new Calendar();
		data.Setting();
	}
	
	private void Setting()
	{				
		
		for (int i = 1950; i <= 2020; i++)			
		{			
			MonthCalendarMap = new HashMap<String, Map<String, LinkedList<String>>>();			
			for (int j = 1; j <= 12; j++)
			{
				DayCalendarMap = new HashMap<String, LinkedList<String>>();
				for (int m = 1; m <= 31; m++)
				{				
					DayCalendarMap.put(Integer.toString(m), new LinkedList<String>());
				}
				MonthCalendarMap.put(Integer.toString(j), DayCalendarMap);				
			}
			YearCalenderMap.put(Integer.toString(i), MonthCalendarMap);
		}
		
		setTitle("2015031090 심재철");		
		
		s = new SettingPanel(this);		
		
		add(s);
		setSize(500, 300);
		setVisible(true);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);		
	}	
}
