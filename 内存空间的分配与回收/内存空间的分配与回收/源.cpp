#include<iostream>
#include<string>
using namespace std;

//�ѷ����
struct Allocation_Table {
	int initial_address;//��ʼ��ַ
	int length;//����
	int allocation_table_status = 0;//����Ϊ�գ���ʾδ������(���̺�)
	Allocation_Table *next;
};

//�������� ���
struct Free_Node {
	string process_name;//������
	int number;//���̺�
	bool status;//true��ʾ���У�false��ʾ�ѷ���
	int size;//������С
	int address;//�׵�ַ
	Free_Node *prev;
	Free_Node *next;

	//���캯��
	Free_Node(){ }

	Free_Node(string process_name, int number, int size)
	{
		this->process_name = process_name;
		this->number = number;
		this->size = size;
	}
};

class MemorySpace {
	Free_Node *head_free;
	Allocation_Table *head_allocation;
public:
	MemorySpace(int n);
	~MemorySpace();
	void ApplicateSpace();//�ڴ����� �����������̺ţ�������С
	void AddTableNode(int initial_address,int length, int allocation_table_status);//�Ǽ��ѷ�������
	void RemoveTableNode(int allocation_table_status);//ɾ���ѷ��������
	void FreeSpace();//�ڴ����
	void MergeFreeSpace(int number);//�ϲ����� ���������̺�
	void PrintLink(bool flag);//��ӡ���������н�㣨�����ѷ���Ŀ�������
	void PrintTable();//��ӡ�ѷ����
};

MemorySpace::MemorySpace(int n)//�������������
{
	head_free = new Free_Node();
	head_free->status = false;
	head_free->next = NULL;
	head_free->size = n;//����ʣ������

	Free_Node *firstnode = new Free_Node("freespace",0,n);//����ĵ�һ����㣬δ���䣬�ռ�Ϊn
	firstnode->address = 0;//������ʼ��ַΪ0
	firstnode->status = true;//δ����
	firstnode->next = NULL;

	firstnode->prev = head_free;//ָ��ͷ��㣬����˫������
	head_free->next = firstnode;//ָ������ĵ�һ�����

	head_allocation = new Allocation_Table();
	head_allocation->next = NULL;
}

MemorySpace::~MemorySpace()//��������
{
	Free_Node *q = head_free;
	while (head_free)
	{
		q = head_free;
		head_free = head_free->next;
		delete q;
	}
	
	Allocation_Table *p = head_allocation;
	while (head_allocation)
	{
		p = head_allocation;
		head_allocation = head_allocation->next;
		delete p;
	}
}

void MemorySpace::ApplicateSpace()
{
	bool flag = false;//�����ж��Ƿ����ռ�ɹ�
	//�����µĿռ�
	string name;
	int number;
	int size;
	cout << "������������Ľ����������̺ţ������С��";
	cin >> name >> number >> size;
	if (size <= head_free->size)//�ж��ڴ�ռ�ʣ�����������Ƿ���ڵ�����������(������Ƭ)
	{
		//cout << p_new->process_name << "  " << p_new->number << " " << p_new->size;
		Free_Node *p_work = head_free->next;
		while (p_work)
		{
			if (p_work->status == true && p_work->size > size)//������ڿ��� && �ռ� > ������
			{
				Free_Node *p_new = new Free_Node(name, number, size);//�����µĿռ䣬�����ù��캯��
				p_new->status = false;//���״̬Ϊ�ѷ���
				//˫�������в���һ�����
				p_new->next = p_work;
				p_new->prev = p_work->prev;
				p_work->prev->next = p_new;
				p_work->prev = p_new;

				p_work->size = p_work->size - p_new->size;//p_work���ڿռ�����p_new���������ռ��Ǵ�p_work�Ϸָ������

				p_new->address = p_work->address;//��ʼ��ַ������
				p_work->address = p_work->address + size;
				AddTableNode(p_new->address,size,p_new->number);//�Ǽ��ѷ�����˵����
				flag = true;//�����־����Ϊ�ɹ�
				break;
			}
			else if (p_work->status == true && p_work->size == size)//������ڿ��� && �ռ� = ������
			{
				//����÷����Ŀ��д�С�պ���ȣ���ֱ��������������Ӧ����Ϣ
				p_work->process_name = name;
				p_work->number = number;
				p_work->size = size;
				p_work->status = false;//���״̬Ϊ�ѷ���
				AddTableNode(p_work->address, size, p_work->number);//�Ǽ��ѷ�����˵����
				flag = true;//�����־����Ϊ�ɹ�
				break;
			}
			p_work = p_work->next;
		}
		if (flag == true)
		{
			cout << "���̺�Ϊ��" << number << " �Ľ��̣���������ɹ���" << endl;
			
		}
		else
			cout << "�����������㣬����ʧ�ܣ�" << endl;
	}
	else
		cout << "�����������㣬����ʧ�ܣ�"<<endl;
}

void MemorySpace::AddTableNode(int initial_address, int length, int allocation_table_status)//�����ѷ���˵����
{
	
	Allocation_Table *q_temp = new Allocation_Table();
	q_temp->allocation_table_status = allocation_table_status;//���̺�
	q_temp->length = length;//������С
	q_temp->initial_address = initial_address;//��ʼ��ַ
	q_temp->next = head_allocation->next;//������ͷ�巨
	head_allocation->next = q_temp;
}

void MemorySpace::RemoveTableNode(int allocation_table_status)
{
	//������ɾ��������
	Allocation_Table *p_prev = head_allocation;//ǰ�����
	Allocation_Table *p_delete = head_allocation->next;//ɾ�����
	while (p_delete)
	{
		if (p_delete->allocation_table_status == allocation_table_status)//�ҵ���Ҫɾ���Ľ�㣬ɾ��
		{
			p_prev->next = p_delete->next;
			delete p_delete;
			break;
		}
		p_prev = p_prev->next;
		p_delete = p_delete->next;
	}
}

void MemorySpace::FreeSpace()//�ͷſռ�
{
	int number;
	bool find = false;//�ҵ���־ true���ҵ���false��δ�ҵ�
	cout << "������ɵĽ��̺ţ�";
	cin >> number;
	Free_Node *p_work = head_free->next;
	//�ҵ����̺�Ϊnumber�Ľ��
	while (p_work)
	{
		if (p_work->number == number)
		{
			find = true; //�ҵ���־����Ϊtrue
			break;
		}
		p_work = p_work->next;
	}
	if (find == true)
	{
		cout << "���̺�Ϊ��" << p_work->number << "  ��������" << p_work->process_name << "���ͷſռ䣡" << endl;
		RemoveTableNode(p_work->number);//���ѷ�����ж�Ӧ�ı�Ŀɾ��
		//ʵ���ͷ�����
		p_work->process_name = "freespace";//�������޸�Ϊ freespace
		p_work->status = true;//δ����
		//p_work->number = 0;����
		head_free->size = head_free->size + p_work->size;//ϵͳʣ���������Ӹո��ͷŵ�����
		MergeFreeSpace(p_work->number);//���úϲ���������
	}
	else
		cout << "���̺�Ϊ " << number <<" �Ľ���δ�ҵ���" << endl;
}

void MemorySpace::MergeFreeSpace(int number)
{
	Free_Node *p_work = head_free->next;
	//�ҵ��Ǹ��ͷ�����ռ�Ľ��
	while (p_work)
	{
		if (p_work->number == number)
			break;
		p_work = p_work->next;
	}

	if (p_work->next == NULL)//����ȫ�������꣬��ʣ��ռ�
	{
		p_work->number = 0;//ָ�벻��Ҫ�䶯������Ӧ���Ľ��̺���Ϊ0
	}
	else if (p_work->prev->status == false && p_work->next->status == false )//�ͷŷ�����ǰ�����ڽӷ��������ѷ�������û�кϲ����������
	{
		p_work->number = 0;//ָ�벻��Ҫ�䶯������Ӧ���Ľ��̺���Ϊ0
	}
	else if (p_work->prev->status == true && p_work->next->status == false)//�ͷŷ�����ǰ�ڽӷ����ǿ����������ڽӷ������ѷ�������
	{
		p_work->prev->size = p_work->prev->size + p_work->size;
		p_work->prev->next = p_work->next;
		p_work->next->prev = p_work->prev;
		delete p_work;
	}
	else if (p_work->prev->status == false && p_work->next->status == true)//�ͷ�����ǰ�ڽӷ������ѷ����������ڽӷ����ǿ�������
	{
		/*p_work->size = p_work->size + p_work->next->size;
		Free_Node *p_temp = p_work->next;
		p_work->next = p_temp->next;
		p_temp->next->prev = p_work;
		delete p_temp;
		p_work->number = 0;*/
		p_work->size = p_work->size + p_work->next->size;
		Free_Node *p_temp = p_work->next;
		if (p_temp->next == NULL)//���������Ϊ˫����������һ����㣬����Ҫ����ǰ��ָ��
		{
			p_work->next = NULL;
		}
		else {
			p_work->next = p_temp->next;//�����������˫����������һ����㣬��Ҫ����ǰ��ָ��
			p_temp->next->prev = p_work;
		}
		delete p_temp;
		p_work->number = 0;

	}
	else if (p_work->prev->status == true && p_work->next->status == true)//�ͷ�����ǰ�����ڽӷ������ǿ�������
	{
		/*Free_Node *p_tempprev = p_work->prev;
		Free_Node *p_tempnext = p_work->next;
		p_tempprev->size = (p_tempprev->size + p_work->size + p_tempnext->size);
		p_tempnext->next->prev = p_tempprev;
		p_tempprev->next = p_tempnext->next;
		delete p_work;
		delete p_tempnext;*/

		Free_Node *p_tempprev = p_work->prev;
		Free_Node *p_tempnext = p_work->next;
		p_tempprev->size = (p_tempprev->size + p_work->size + p_tempnext->size);
		if (p_tempnext->next == NULL)//�ͷ����ĺ��ڽӷ����ǿ�����������Ҫ����ǰ��ָ��
		{
			p_tempprev->next = NULL;
		}
		else {
			p_tempnext->next->prev = p_tempprev;//�ͷ����ĺ��ڽӷ����ǿ�����������Ҫ����ǰ��ָ��
			p_tempprev->next = p_tempnext->next;
		}
		delete p_work;
		delete p_tempnext;
	}
}

void MemorySpace::PrintLink(bool flag)
{
	if (flag == true)//�����������
	{
		cout << "�����е����з�����" << endl;
		Free_Node *p_temp = head_free->next;
		while (p_temp)
		{
			cout << "��������" <<p_temp->process_name << " ���̺ţ�" << p_temp->number << " ���з�����С��" << p_temp->size << " ����״̬��" << p_temp->status <<" �׵�ַ��"<<p_temp->address<< endl;
			p_temp = p_temp->next;
		}
	}
	else//���δ����ķ���
	{
		cout << "�����еĿ��з�����" << endl;
		Free_Node *p_temp = head_free->next;
		while (p_temp)
		{
			if(p_temp->status == true)
				cout << "��������" <<p_temp->process_name << " ���̺ţ�" << p_temp->number << " ������С��" << p_temp->size << " ����״̬��" << p_temp->status << " �׵�ַ��" << p_temp->address << endl;
			p_temp = p_temp->next;
		}
	}
}

void MemorySpace::PrintTable()//����ѷ���˵����
{
	cout << "�ѷ���˵����" << endl;
	Allocation_Table *p_work = head_allocation->next;
	while (p_work)
	{
		cout << "���̺ţ�" << p_work->allocation_table_status << " ��ʼ��ַ��" << p_work->initial_address << " ������С��" << p_work->length << endl;
		p_work = p_work->next;
	}
}

int main()
{
	int space;
	cout << "����������Ŀռ��С��";
	cin >> space;
	MemorySpace Space(space);
	int choice;
	do
	{
		cout << endl;
		cout << "        �������� �� 1. ������������ռ�  �� �������" << endl;
		cout << "        �������� �� 2. �����ͷ�����ռ�  �� �������" << endl;
		cout << "        �������� �� 3. ��ӡ�������з���  �� �������" << endl;
		cout << "        �������� �� 4. ��ӡ������з���  �� �������" << endl;
		cout << "        �������� �� 5.��ӡ�ѷ�����˵���� �� �������" << endl;
		cout << "        �������� �� 0.��              �� �� �������" << endl << endl;
		cout << "�������";
		cin >> choice;
		switch (choice)
		{
		case 1:
			Space.ApplicateSpace();
			break;
		case 2:
			Space.FreeSpace();
			break;
		case 3:
			Space.PrintLink(true);
			break;
		case 4:
			Space.PrintLink(false);
			break;
		case 5:
			Space.PrintTable();
			break;
		case 0:
			break;
		default:
			cout << "���������" << endl;
			exit(1);
			break;
		}
	} while (choice != 0);
	return 0;
}


//����
//1.�ڴ�ȫ�������꣬�޿��пռ�ʱ��ɾ�����һ����㱨��
//2.ͬ�����⣬���ͷ�����ǰ�ڽӷ������ѷ����������ڽӷ����ǿ����������������һ�����Ϊ��������ɾ�������ڶ�����㣬����
//3.ͬ�����⣬���ͷ�����ǰ�����ڽӷ������ǿ����������������һ�����͵������������Ϊ��������ɾ�������ڶ�����㣬����
//4.��һ�д���һֱ��ִ�У�
//5.FreeSpace()�����198�У�ɾ������ʱ���Ҳ�����Ӧ�Ľ��̺� ��
//6.��ʼ��ʱ��������� ���õ��� ������ ���ǵ��ڴ����ɾ����� ����д��һ���ʱ�� ������ ��Ϊ ˫������
//���
//˫������ɾ�����һ��������
//д��ʱ���е��Ǹ�����ֵд���˵��ڣ��¶ϵ��ر�����ʱ��if�ж�û���⣬������Ͳ�ִ��

//��������
//200
//1
//pro1 1 30
//1
//pro2 2 70
//1
//pro3 3 15
//1
//pro4 4 30
//1
//pro5 5 5
//1
//pro6 6 30
