
#include "stdafx.h"
#include "New_struct.h"
#include "cstdlib"

int get_adr_opr(MTable::iterator &itr, l_operand &opr, MTable table, int *counts);
void null_l_oprand(l_operand& opr);
void print_operand(l_operand opr);
int get_offset(l_operand opr1, l_operand opr2, int comand_num, ASSUME_TABLE assume, int call_stack);

void get_reg_opr(l_operand &opr, string reg)
{
	if (reg == "AL")
	{
		opr.reg.is_reg = 1;
		opr.reg.r_bit = 8;
		opr.reg.r_num = 0;
	}
	else
		if (reg == "AH")
		{
			opr.reg.is_reg = 1;
			opr.reg.r_bit = 8;
			opr.reg.r_num = 4;
		}
		else
			if (reg == "CL")
			{
				opr.reg.is_reg = 1;
				opr.reg.r_bit = 8;
				opr.reg.r_num = 1;
			}
			else
				if (reg == "CH")
				{
					opr.reg.is_reg = 1;
					opr.reg.r_bit = 8;
					opr.reg.r_num = 5;
				}
				else
					if (reg == "DL")
					{
						opr.reg.is_reg = 1;
						opr.reg.r_bit = 8;
						opr.reg.r_num = 2;
					}
					else
						if (reg == "DH")
						{
							opr.reg.is_reg = 1;
							opr.reg.r_bit = 8;
							opr.reg.r_num = 6;
						}
						else
							if (reg == "BL")
							{
								opr.reg.is_reg = 1;
								opr.reg.r_bit = 8;
								opr.reg.r_num = 3;
							}
							else
								if (reg == "BH")
								{
									opr.reg.is_reg = 1;
									opr.reg.r_bit = 8;
									opr.reg.r_num = 7;
								}
								else
									if (reg == "AX")
									{
										opr.reg.is_reg = 1;
										opr.reg.r_bit = 16;
										opr.reg.r_num = 0;
									}
									else
										if (reg == "CX")
										{
											opr.reg.is_reg = 1;
											opr.reg.r_bit = 16;
											opr.reg.r_num = 1;
										}
										else
											if (reg == "DX")
											{
												opr.reg.is_reg = 1;
												opr.reg.r_bit = 16;
												opr.reg.r_num = 2;
											}
											else
												if (reg == "BX")
												{
													opr.reg.is_reg = 1;
													opr.reg.r_bit = 16;
													opr.reg.r_num = 3;
												}
												else
													if (reg == "SP")
													{
														opr.adr_reg1.adr_bit = 16;
														opr.adr_reg1.is_adr_reg = 1;
														opr.adr_reg1.adr_num = 4;
													}
													else
														if (reg == "BP")
														{
															opr.adr_reg1.adr_bit = 16;
															opr.adr_reg1.is_adr_reg = 1;
															opr.adr_reg1.adr_num = 5;
														}
														else
															if (reg == "SI")
															{
																opr.adr_reg1.adr_bit = 16;
																opr.adr_reg1.is_adr_reg = 1;
																opr.adr_reg1.adr_num = 6;
															}
															else
																if (reg == "DI")
																{
																	opr.adr_reg1.adr_bit = 16;
																	opr.adr_reg1.is_adr_reg = 1;
																	opr.adr_reg1.adr_num = 7;
																}
}

int new_struct_check(MTable table, SS_table & ss_tab, Assume_vector & V_assume, Label_vector & lvector, Var_vector & vvector, Segment_vector & svector, Proc_vector & pvector)
{
	ofstream f;
	MTable::iterator itr = table.end();
	ASSUME_TABLE assume;
	int clk = 1;
	int ascount = 0;
	size_t *string_offset;
	T_proc t_pr;
	T_segments t_seg;
	int in_seg = 0;
	int ind;
	int counts;
	SS_item s_item = { 0 };
	vector<l_operand> vlopr;
	var_seg_vect VSV;
	var_seg VS;
	string vs_cur_seg;
	MTable::iterator first_word = table.begin();
	proc_seg PS;
	prov_seg_vect PSV;
	lab_seg LS;
	lab_seg_vect LSV;
	int seg_end = 0;
	string cur_seg;
	string_offset = new size_t[(--itr)->Get_string_number() + 1];
	f.open("List1.lst");
	for (itr = table.begin(); itr != table.end(); clk++)
	{
		MTable::iterator nitr = itr;
		size_t nxst = itr->Get_string_number() + 1;
		if (nitr->Get_type() == 7)
		{
			if ((++nitr)->Get_type() == 6)
			{
				string c1_word = nitr->Get_word();
				if (c1_word == "DB")
					VS.type = 1;
				else
					if (c1_word == "DW")
						VS.type = 2;
					else
						if (c1_word == "DD")
							VS.type = 4;
				VS.name = itr->Get_word();
				VS.segment = vs_cur_seg;
				VSV.push_back(VS);
			}
			else
			{
				if (!strcmp(nitr->Get_word(), "PROC"))
				{
					PS.pname = itr->Get_word();
					PS.sname = vs_cur_seg;
					if (!strcmp((++nitr)->Get_word(), "FAR"))
						PS.ptype = 4;
					else
						PS.ptype = 2;
					PSV.push_back(PS);
				}
				else
				{
					nitr = itr;
					if (!strcmp((++nitr)->Get_word(), "SEGMENT"))
						vs_cur_seg = itr->Get_word();
					else
					{
						nitr = itr;
						if (!strcmp((++nitr)->Get_word(), ":"))
						{
							LS.lname = itr->Get_word();
							LS.sname = vs_cur_seg;
							LSV.push_back(LS);
						}
					}
				}
		
			}
		}
		while (itr != table.end() && itr->Get_string_number() < nxst)
			itr++;
	}

	for (auto it = VSV.begin(); it != VSV.end(); it++)
	{
		std::cout << "NAME: " << it->name << "	TYPE: " << it->type << "	 SEGMENT: " << it->segment << endl;;
	}
	std::cout << endl;

	for (auto it = PSV.begin(); it != PSV.end(); it++)
	{
		std::cout << "NAME: " << it->pname << "	 SEGMENT: " << it->sname << endl;;
	}
	std::cout << endl;

	for (auto it = LSV.begin(); it != LSV.end(); it++)
	{
		std::cout << "NAME: " << it->lname << "	 SEGMENT: " << it->sname << endl;;
	}
	std::cout << endl;


	clk = 1;
	for (itr = table.begin(); itr != table.end();)
	{
		string word = itr->Get_word();
		size_t type = itr->Get_type();
		SS_item s_item = { 0 };

		size_t next_string = itr->Get_string_number() + 1;
		if (word == "ASSUME")
		{
			MTable::iterator asu = itr;
			assume["CS"] = "nothing";
			assume["DS"] = "nothing";
			assume["ES"] = "nothing";
			assume["FS"] = "nothing";
			assume["GS"] = "nothing";
			assume["SS"] = "nothing";
			if ((++asu)->Get_string_number() == itr->Get_string_number())
			{
				while (itr != table.end() && itr->Get_string_number() < next_string)
				{
					asu = itr;
					if (++asu != table.end() && ++asu != table.end() && ++asu != table.end())
					{
						if (++asu != table.end())
						{
							asu = itr;
							if ((++asu)->Get_type() == 3 && !strcmp((++asu)->Get_word(), ":") && (++asu)->Get_type() == 7 && (!strcmp((++asu)->Get_word(), ",") || asu->Get_string_number() == next_string))
							{
								assume[(++itr)->Get_word()] = (--asu)->Get_word();
								itr = ++asu;
							}
							else
							{
								while (asu->Get_string_number() != next_string && asu != table.end())
									asu++;
								itr = asu;
								std::cout << "ASSUME ERROR 2 " << clk << endl;
							}
						}
						else
						{
							asu = itr;
							if ((++asu)->Get_type() == 3 && !strcmp((++asu)->Get_word(), ":") && (++asu)->Get_type() == 7)
							{
								assume[(++itr)->Get_word()] = asu->Get_word();
								itr = ++asu;
							}
							else
							{
								while (asu->Get_string_number() != next_string && asu != table.end())
									asu++;
								itr = asu;
								std::cout << "ASSUME ERROR 3 " << clk << endl;
							}
						}
					}
				}
				itr--;
				ascount++;
				V_assume.push_back(assume);
				string_offset[clk] = string_offset[clk - 1];
			}
		}
		else
		{
			if (type == 7)
			{
				MTable::iterator nitr = itr;
				string nword;
				nword = (++nitr)->Get_word();
				string_offset[clk] = string_offset[clk - 1];
				if (nword == "SEGMENT")
				{
					// new segment
					cur_seg = itr->Get_word();
					in_seg++;
					t_seg.name = word;
					t_seg.type = 0;
					t_seg.len = 0;
					s_item.rec.label_number = 1;
					s_item.rec.mem_code_num = 2;
					s_item.rec.mem_code_len = 1;
					string_offset[clk - 1] = 0;
					string_offset[clk] = 0;
					if (in_seg > 1)
					{
						std::cout << "SEGMENT > 1 in row " << clk << endl;
					}
					itr = nitr;
				}
				else
				{
					if (nitr->Get_type() == 6 && in_seg > 0)
					{
						string cur_word = nitr->Get_word();
						T_Variables t_var;
						t_var.name = word;
						t_var.segment = t_seg.name;
						t_var.offset = string_offset[clk - 1];
						// add variable
						s_item.rec.label_number = 1;
						s_item.rec.mem_code_num = 2;
						s_item.rec.mem_code_len = 1;
						if (cur_word == "DB")
						{
							t_var.size = 1;
							cur_word = (++nitr)->Get_word();
							if (cur_word[0] == '\'')
							{
								int i = 1;
								int cur_s;
								while (cur_word[i] != '\'' && cur_word[i] != '\0')
								{
									char buf[6] = { 0 };
									cur_s = atoi(&(cur_word[i]));
									_itoa_s(cur_s, buf, 6, 16);
									t_var.value += buf;
									t_var.value += " ";
									i++;
									string_offset[clk]++;
								}
							}
							else
							{
								int i = 1;
								int cur_s;
								char buf[6] = { 0 };
								cur_s = atoi(nitr->Get_word());
								_itoa_s(cur_s, buf,6,16);
								t_var.value = buf;
								string_offset[clk]++;
							}
							//cout << t_var.value << endl;
						}
						else
							if (cur_word == "DW")
							{
								nitr++;
								int i = 1;
								int cur_s;
								char buf[6] = { 0 };
								cur_s = atoi(nitr->Get_word());
								_itoa_s(cur_s, buf, 6, 16);
								t_var.value = buf;
								t_var.size = 2;
								string_offset[clk] += 2;
							}
							else
							{
								nitr++;
								int i = 1;
								int cur_s;
								char buf[6] = { 0 };
								cur_s = atoi(nitr->Get_word());
								_itoa_s(cur_s, buf, 6, 16);
								t_var.value = buf;
								t_var.size = 4;
								string_offset[clk] += 4;
							}
						itr = nitr;
						vvector.push_back(t_var);
						s_item.rec.f_operand_len = 1;
						s_item.rec.f_operand_num = 3;
					}
					else
					{
						if (nword == "ENDS")
						{
							string cur_word = itr->Get_word();
							seg_end = 1;
							if (t_seg.name == cur_word)
							{
								in_seg--;
								s_item.rec.label_number = 1;
								s_item.rec.mem_code_num = 2;
								s_item.rec.mem_code_len = 1;
								string_offset[clk] = string_offset[clk - 1];
								t_seg.len = string_offset[clk];
								svector.push_back(t_seg);
								string_offset[clk] = 0;
								if (in_seg < 0)
								{
									std::cout << "SEGMENT < 0 in row " << clk << endl;
								}
							}
							else 
								std::cout << "NOT CORRECT SEGMENT NAME in row " << clk << endl;
							itr = nitr;
						}
						else
							if (nword == "EQU")
							{
								string_offset[clk] = string_offset[clk - 1];
								s_item.rec.label_number = 1;
								s_item.rec.f_operand_num = 3;
								s_item.rec.f_operand_len = 1;
								s_item.rec.mem_code_num = 2;
								s_item.rec.mem_code_len = 1;
								itr = ++nitr;
							}
							else
								if (nword == ":")
								{
									T_Lables t_lab;
									string_offset[clk] = string_offset[clk-1];
									t_lab.name = itr->Get_word();
									t_lab.offset = string_offset[clk];
									t_lab.segment = t_seg.name;
									s_item.rec.label_number = 1;
									ind = 1;
									counts = 2;
									itr = nitr;
									lvector.push_back(t_lab);
								}
								else
									if (nword == "PROC")
									{
										string_offset[clk] = string_offset[clk - 1];
										t_pr.name = itr->Get_word();
										t_pr.offset = string_offset[clk];
										t_pr.segment = t_seg.name;
										s_item.rec.label_number = 1;
										s_item.rec.mem_code_len = 1;
										s_item.rec.mem_code_num = 2;
										itr = nitr;
									}
									else
										if (nword == "ENDP")
										{
											s_item.rec.label_number = 1;
											s_item.rec.mem_code_len = 1;
											s_item.rec.mem_code_num = 2;
											string_offset[clk] = string_offset[clk - 1];
											t_pr.len = t_pr.offset - string_offset[clk];
											pvector.push_back(t_pr);
											itr = nitr;
										}
					}
				}
			}//type == 7
			else
			{
				if (word == "END")
				{
					s_item.rec.mem_code_len = 1;
					s_item.rec.mem_code_num = 1;
				}
				else
				{
					if (itr->Get_type() == 5)
					{
						MTable::iterator nitr = itr;
						nitr++;
						s_item.rec.mem_code_len = 1;
						s_item.rec.mem_code_num = counts++;
						ind += 2;
						if (word == "PUSH")
						{
							l_operand opr;
							l_operand opr1;
							string cur_word = nitr->Get_word();
							size_t cur_type = nitr->Get_type();
							null_l_oprand(opr1);
							null_l_oprand(opr);
							nitr++;
							s_item.rec.mem_code_len = 1;
							s_item.rec.mem_code_num = ind++;
							if (cur_type == 22 && (nitr == table.end() || nitr->Get_string_number() == next_string))
							{
								opr.reg.is_reg = 1;
								opr.reg.r_bit = 16;
								opr.row = clk;
								opr1.row = clk;
								opr.comand_num = 2;
								opr1.comand_num = 2;
								s_item.rec.f_operand_len = 1;
								s_item.rec.f_operand_num = counts++;
								if (cur_word == "AX")
									opr.reg.r_num = 0;
								else
									if (cur_word == "CX")
										opr.reg.r_num = 1;
									else
										if (cur_word == "DX")
											opr.reg.r_num = 2;
										else
											if (cur_word == "BX")
												opr.reg.r_num = 3;
								string_offset[clk] = string_offset[clk - 1] + 1;
								opr1.row = clk;
								opr.row = clk;
								vlopr.push_back(opr);
								vlopr.push_back(opr1);
							}
							else
							{
								if (cur_type == 23 && (nitr == table.end() || nitr->Get_string_number() == next_string))
								{
									opr.adr_reg1.is_adr_reg = 1;
									opr.adr_reg1.adr_bit = 16;
									opr.row = clk;
									opr1.row = clk;
									opr.comand_num = 2;
									opr1.comand_num = 2;
									opr.row = clk;
									opr1.row = clk;
									s_item.rec.f_operand_len = 1;
									s_item.rec.f_operand_num = counts++;
									if (cur_word == "SP")
										opr.adr_reg1.adr_num = 4;
									else
										if (cur_word == "BP")
											opr.adr_reg1.adr_num = 5;
										else
											if (cur_word == "SI")
												opr.adr_reg1.adr_num = 6;
											else
												if (cur_word == "DI")
													opr.adr_reg1.adr_num = 7;
									string_offset[clk] = string_offset[clk - 1] + 1;
									opr1.row = clk;
									opr.row = clk;
									vlopr.push_back(opr);
									vlopr.push_back(opr1);
								}
								else
								{
									//PUSH ERROR
									std::cout << "UNKNOWN PARAMETERS IN PUSH IN LINE " << clk << endl;
									string_offset[clk] = string_offset[clk - 1];
									itr++;
									s_item.rec.f_operand_num = ind;
									while (itr != table.end() && itr->Get_string_number() != next_string)
									{
										s_item.rec.f_operand_len++;
										itr++;
									}
									itr--;
								}
							}
							
							itr = nitr;
							itr--;
						}
						else
						{
							if (word == "POP")
							{
								auto it = VSV.begin();
								l_operand opr1;
								l_operand opr2;
								string cur_word = nitr->Get_word();
								size_t cur_type = nitr->Get_type();
								int is_var_pr = 0;
								string var_name;
								int ptr_error = 0;
								null_l_oprand(opr1);
								null_l_oprand(opr2);
								opr1.comand_num = 3;
								opr2.comand_num = 3;
								if (cur_word == "WORD")
								{
									nitr++;
									cur_word = nitr->Get_word();
									if (cur_word == "PTR")
									{
										opr1.ptr.is_ptr = 1;
										opr1.ptr.ptr_type = 2;
									}
									else
										ptr_error = 1;
									nitr++;
									cur_word = nitr->Get_word();
									cur_type = nitr->Get_type();
								}
								if (cur_type == 7)
								{
									counts++;
									var_name = nitr->Get_word();
									nitr++;
									is_var_pr = 1;
									for (it; it != VSV.end(); it++)
									{
										if (it->name == var_name)
										{
											if (it->type != 2)
												it = VSV.end();
											break;
										}
									}
								}
								if (!strcmp(nitr->Get_word(), "[") && it != VSV.end())
								{
									int counts2 = 0;
									nitr++;
									if (!get_adr_opr(nitr, opr1, table, &counts2))
									{
										if (is_var_pr)
										{
											opr1.undentf.is_undtf = 1;
											opr1.undentf.seg = cur_seg;
											opr1.undentf.type = 2;
											if (opr1.adr_reg1.adr_num == 3 || opr1.adr_reg2.adr_num == 3)
											{
												auto it1 = assume.begin();
												for (it1; it1 != assume.end(); it1++)
													if (it1->second == it->segment)
													{
														if (it1->first != "DS")
														{
															string_offset[clk] = string_offset[clk - 1] + 5;
														}
														else
															string_offset[clk] = string_offset[clk - 1] + 4;
														break;
													}
												if (it1 == assume.end() || ptr_error)
												{
													string_offset[clk] = string_offset[clk - 1];
													std::cout << "ERROR POP in line " << clk << endl;
												}
												else
												{
													opr1.row = clk;
													opr2.row = clk;
													vlopr.push_back(opr1);
													vlopr.push_back(opr2);
												}
											}
											else
												if (opr1.adr_reg1.adr_num == 5 || opr1.adr_reg2.adr_num == 5)
												{
													auto it1 = assume.begin();
													for (it1; it1 != assume.end(); it1++)
														if (it1->second == it->segment)
														{
															if (it1->first != "SS")
															{
																string_offset[clk] = string_offset[clk - 1] + 5;
															}
															else
																string_offset[clk] = string_offset[clk - 1] + 4;
															break;
														}
													if (it1 == assume.end() || ptr_error)
													{
														string_offset[clk] = string_offset[clk - 1];
														std::cout << "ERROR POP in line " << clk << endl;
													}
												}
												else
												{
													opr1.row = clk;
													opr2.row = clk;
													vlopr.push_back(opr1);
													vlopr.push_back(opr2);
												}
										}
										else
											string_offset[clk] = string_offset[clk - 1] + 2;
									}
									else
									{
										std::cout << "INVALID ADRESSATION OR NOT CORRECT POP FORM in line " << clk << endl;
										string_offset[clk] = string_offset[clk - 1];
									}
									nitr--;
									itr = nitr;
								}
								else
								{
									std::cout << "INVALID ADRESSATION OR NOT CORRECT POP FORM in line " << clk << endl;
									string_offset[clk] = string_offset[clk - 1];
									while (itr != table.end() && itr->Get_string_number() != next_string)
										itr++;
									itr--;
								}
							}
							else
							{
								if (word == "MOV")
								{
									l_operand opr1;
									l_operand opr2;
									string cur_word = nitr->Get_word();
									size_t cur_type = nitr->Get_type();
									nitr++;
									null_l_oprand(opr1);
									null_l_oprand(opr2);
									cur_word = nitr->Get_word();
									opr1.comand_num = 4;
									opr2.comand_num = 4;
									if ((cur_type == 22 || cur_type == 23) && cur_word == ",")
									{
										string r_name = (--nitr)->Get_word();
										size_t c_type;
										MTable::iterator mitr;
										get_reg_opr(opr1, r_name);
										nitr++;
										mitr = nitr;
										c_type = (++nitr)->Get_type();
										cur_word = nitr->Get_word();
										s_item.rec.f_operand_num = counts++;
										s_item.rec.f_operand_len = 1;
										if ( (c_type == 22 || c_type == 23) && (++nitr)->Get_string_number() == next_string)
										{
												string_offset[clk] = string_offset[clk - 1] + 2;
												itr = nitr;
												itr--;
										}
										else
										{
											//nitr--;
											if (cur_word == "OFFSET")
											{
												opr2.is_adr_com = 1;
												c_type = (++nitr)->Get_type();
												cur_word = nitr->Get_word();
												if (c_type == 7)
												{
													s_item.rec.s_operand_len = 2;
													s_item.rec.s_operand_num = counts++;
													auto it = VSV.begin();
													for (it; it != VSV.end(); it++)
														if (cur_word == it->name)
														{
															opr2.row = clk;
															opr1.row = clk;
															opr2.undentf.is_undtf = 1;
															opr2.undentf.seg = it->segment;
															opr2.undentf.type = it->type;
															string_offset[clk] = string_offset[clk - 1] + 3;
															vlopr.push_back(opr1);
															vlopr.push_back(opr2);
															break;
														}
													if (it == VSV.end())
													{
														string_offset[clk] = string_offset[clk - 1];
														std::cout << "MOV OFFSET UNKNOWN VARIABLE in row " << clk << endl;
														while (itr != table.end() && itr->Get_string_number() != next_string)
															itr++;
														itr--;
													}
													else
													{
														if ((++nitr)->Get_string_number() == next_string)
															itr = --nitr;
														else
														{
															std::cout << "MOV OFFSET BAD OPERANDS in row " << clk << endl;
															while (itr != table.end() && itr->Get_string_number() != next_string)
															{
																s_item.rec.s_operand_len++;
																itr++;
															}
															itr--;
														}
													}
												}
												else
												{
													std::cout << "MOV OFFSET BAD OPERANDS in row " << clk << endl;
													string_offset[clk] = string_offset[clk - 1];
													while (itr != table.end() && itr->Get_string_number() != next_string)
														itr++;
													itr--;
												}
											}
											else
										{
											int ptr_error = 0;
											if (cur_word == "WORD" || cur_word == "DWORD" || cur_word == "BYTE")
											{
												string ttype = cur_word;
												nitr++;
												cur_word = nitr->Get_word();
												if (cur_word == "PTR")
												{
													opr2.ptr.is_ptr = 1;
													if (ttype == "WORD")
														opr2.ptr.ptr_type = 2;
													else
														if (ttype == "DWORD")
															opr2.ptr.ptr_type = 4;
														else
															if (ttype == "BYTE")
																opr2.ptr.ptr_type = 1;
												}
												else
													ptr_error = 1;
												nitr++;
												cur_word = nitr->Get_word();
												c_type = nitr->Get_type();
											}

											if (c_type == 1 || c_type == 9 || c_type == 10 || cur_word == "(" || cur_word == "-")
											{
												int fst = 0;
												int skb = 0;
												int sym = 0;
												int nm = 0;
												int err = 0;
												int opsk = 0;
												if (cur_word == "(")
												{
													skb++;
													opsk = 1;
												}
												else
													if (cur_word != "-")
														nm++;
												nitr++;
												s_item.rec.s_operand_len++;
												while (nitr != table.end() && nitr->Get_string_number() != next_string)
												{
													c_type = nitr->Get_type();
													cur_word = nitr->Get_word();
													if (c_type == 1 || c_type == 9 || c_type == 10)
														if (nm == 0)
														{
															if (fst)
																sym--;
															else
																fst = 1;
															nm++;
															opsk = 0;

														}
														else
														{
															opsk = 0;
															sym--;
															nm++;
															err = 1;
														}
													else
														if (cur_word == ")")
														{
															opsk = 0;

															if (skb > 0)
																skb--;
															else
																err = 1;
														}
														else
															if (cur_word == "(")
															{
																skb++;
																opsk = 1;
															}
															else
																if (cur_word == "+" || cur_word == "*" || cur_word == "/")
																{
																	MTable::iterator itr2 = nitr;
																	if ((++itr2) != table.end())
																	{
																		string nxw = itr2->Get_word();
																		if (nxw == ")")
																		{
																			err = 1;
																		}
																		if (sym == 0)
																		{
																			sym++;
																			nm--;
																			opsk = 0;

																		}
																		else
																		{
																			sym++;
																			nm--;
																			err = 1;
																			opsk = 0;

																		}
																	}
																	else
																	{
																		err = 1;
																		break;
																	}
																}
																else
																	if (cur_word == "-")
																	{
																		if (opsk == 0)
																		{
																			if (sym > 2)
																				err = 1;
																			else
																			{
																				MTable::iterator itr2 = nitr;
																				if ((++itr2) != table.end())
																				{
																					if (!(itr2->Get_type() == 1 || itr2->Get_type() == 9 || itr2->Get_type() == 10))
																					{
																						string nxw1 = itr2->Get_word();
																						if (nxw1 != "(" && nxw1 != "-")
																							sym++;
																					}
																				}
																				else
																				{
																					err = 1;
																					break;
																				}


																			}
																		}
																		opsk = 0;
																	}
																	else
																		err = 1;
													s_item.rec.s_operand_len++;
													nitr++;
												}
												if (!(skb == 0 && sym == 0 && nm == 1))
													err = 1;
												if (err || ptr_error)
												{
													std::cout << "WAITING FOR PTR OF TOO MANY SYMBOLS in row " << clk << endl;
													string_offset[clk] = string_offset[clk - 1];
												}
												else
												{
													opr1.row = clk;
													opr2.row = clk;
													opr2.iconst.is_const = 1;
													opr2.iconst.value = 2;
													vlopr.push_back(opr1);
													vlopr.push_back(opr2);
													string_offset[clk] = string_offset[clk - 1] + 3;
												}
												itr = --nitr;
											}
											else
											{
												auto it = VSV.begin();
												int is_var_pr = 0;
												string var_name;
												int ptr_error = 0;
												nitr = mitr;
												nitr++;
												cur_word = nitr->Get_word();
												c_type = nitr->Get_type();
												if (cur_word == "WORD")
												{
													nitr++;
													cur_word = nitr->Get_word();
													if (cur_word == "PTR")
													{
														opr2.ptr.is_ptr = 1;
														opr2.ptr.ptr_type = 2;
													}
													else
														ptr_error = 1;
													nitr++;
													cur_word = nitr->Get_word();
													c_type = nitr->Get_type();
												}
												if (c_type == 7)
												{
													counts++;
													var_name = nitr->Get_word();
													nitr++;
													is_var_pr = 1;
													for (it; it != VSV.end(); it++)
													{
														if (it->name == var_name)
														{
															if (it->type != 2)
																it = VSV.end();
															break;
														}
													}
												}
												if (!strcmp(nitr->Get_word(), "[") && it != VSV.end())
												{
													int counts2 = 0;
													nitr++;
													if (!get_adr_opr(nitr, opr2, table, &counts2))
													{
														if (is_var_pr)
														{
															nitr--;
															itr = nitr;
															opr2.undentf.is_undtf = 1;
															opr2.undentf.seg = cur_seg;
															opr2.undentf.type = 2;
															if (opr2.adr_reg1.adr_num == 3 || opr2.adr_reg2.adr_num == 3)
															{
																auto it1 = assume.begin();
																for (it1; it1 != assume.end(); it1++)
																	if (it1->second == it->segment)
																	{
																		if (it1->first != "DS")
																		{
																			string_offset[clk] = string_offset[clk - 1] + 5;
																		}
																		else
																			string_offset[clk] = string_offset[clk - 1] + 4;
																		break;
																	}
																if (it1 == assume.end())
																{
																	string_offset[clk] = string_offset[clk - 1];
																	std::cout << "ERROR! WAITING FOR PTR OF VARIABLE NOT ASSUMED in row " << clk << endl;
																}
																else
																{
																	opr1.row = clk;
																	opr2.row = clk;
																	vlopr.push_back(opr1);
																	vlopr.push_back(opr2);
																}
															}
															else
																if (opr2.adr_reg1.adr_num == 5 || opr2.adr_reg2.adr_num == 5)
																{
																	auto it1 = assume.begin();
																	for (it1; it1 != assume.end(); it1++)
																		if (it1->second == it->segment)
																		{
																			if (it1->first != "SS")
																			{
																				string_offset[clk] = string_offset[clk - 1] + 5;
																			}
																			else
																				string_offset[clk] = string_offset[clk - 1] + 4;
																			break;
																		}
																	if (it1 == assume.end())
																	{
																		string_offset[clk] = string_offset[clk - 1];
																		std::cout << "ERROR! WAITING FOR PTR OF VARIABLE NOT ASSUMED in row " << clk << endl;
																	}
																	else
																	{
																		opr1.row = clk;
																		opr2.row = clk;
																		vlopr.push_back(opr1);
																		vlopr.push_back(opr2);
																	}
																}
														}
														else
															string_offset[clk] = string_offset[clk - 1] + 2;
													}
												}
												else
												{
													std::cout << "INVALID ADRESSATION OR NOT CORRECT MOV FORM in line " << clk << endl;
													string_offset[clk] = string_offset[clk - 1];
													while (itr != table.end() && itr->Get_string_number() != next_string)
														itr++;
													itr--;
												}
											}
										}
										}
									}
									else
										if (cur_type == 21 && cur_word == ",")
										{
											string r_name = (--nitr)->Get_word();
											size_t c_type;
											get_reg_opr(opr1, r_name);
											nitr++;
											c_type = (++nitr)->Get_type();
											cur_word = nitr->Get_word();
											s_item.rec.f_operand_num = counts++;
											s_item.rec.f_operand_len = 1;
											counts++;
											if ( cur_type == 21 && (++nitr)->Get_string_number() == next_string)
											{
												string_offset[clk] = string_offset[clk - 1] + 2;
												itr = nitr;
												itr--;
											}
											else
											{
												nitr--;
											if (cur_word == "OFFSET")
											{
												opr2.is_adr_com = 1;
												c_type = (++nitr)->Get_type();
												cur_word = nitr->Get_word();
												if (c_type == 7)
												{
													s_item.rec.s_operand_len = 2;
													s_item.rec.s_operand_num = counts++;
													auto it = VSV.begin();
													for (it; it != VSV.end(); it++)
														if (cur_word == it->name)
														{
															opr2.row = clk;
															opr1.row = clk;
															opr2.undentf.is_undtf = 1;
															opr2.undentf.seg = it->segment;
															opr2.undentf.type = it->type;
															string_offset[clk] = string_offset[clk - 1] + 2;
															vlopr.push_back(opr1);
															vlopr.push_back(opr2);
															break;
														}
													if (it == VSV.end())
													{
														string_offset[clk] = string_offset[clk - 1];
														std::cout << "MOV OFFSET UNKNOWN VARIABLE in row " << clk << endl;
														while (itr != table.end() && itr->Get_string_number() != next_string)
															itr++;
														itr--;
													}
													else
													{
														if ((++nitr)->Get_string_number() == next_string)
															itr = --nitr;
														else
														{
															std::cout << "MOV OFFSET BAD OPERANDS in row " << clk << endl;
															while (itr != table.end() && itr->Get_string_number() != next_string)
															{
																s_item.rec.s_operand_len++;
																itr++;
															}
															itr--;
														}
													}
												}
												else
												{
													std::cout << "MOV OFFSET BAD OPERANDS in row " << clk << endl;
													while (itr != table.end() && itr->Get_string_number() != next_string)
														itr++;
													itr--;
												}
											}
											else
											{
												int ptr_error = 0;
												if (cur_word == "WORD" || cur_word == "DWORD" || cur_word == "BYTE")
												{
													string ttype = cur_word;
													nitr++;
													cur_word = nitr->Get_word();
													if (cur_word == "PTR")
													{
														opr2.ptr.is_ptr = 1;
														if (ttype == "WORD")
															opr2.ptr.ptr_type = 2;
														else
															if (ttype == "DWORD")
																opr2.ptr.ptr_type = 4;
															else
																if (ttype == "BYTE")
																	opr2.ptr.ptr_type = 1;
													}
													else
														ptr_error = 1;
													nitr++;
													cur_word = nitr->Get_word();
													c_type = nitr->Get_type();
												} 

												if (c_type == 1 || c_type == 9 || c_type == 10 || cur_word == "(" || cur_word == "-")
												{
													int fst = 0;
													int skb = 0;
													int sym = 0;
													int nm = 0;
													int err = 0;
													int opsk = 0;
													if (cur_word == "(")
													{
														skb++;
														opsk = 1;
													}
													else
														if (cur_word != "-")
															nm++;
													nitr++;
													s_item.rec.s_operand_num = counts++;
													s_item.rec.s_operand_len++;
													while (nitr != table.end() && nitr->Get_string_number() != next_string)
													{
														c_type = nitr->Get_type();
														cur_word = nitr->Get_word();
														if (c_type == 1 || c_type == 9 || c_type == 10)
															if (nm == 0)
															{
																if (fst)
																	sym--;
																else
																	fst = 1;
																nm++;
																opsk = 0;

															}
															else
															{
																opsk = 0;
																sym--;
																nm++;
																err = 1;
															}
														else
															if (cur_word == ")")
															{
																opsk = 0;

																if (skb > 0)
																	skb--;
																else
																	err = 1;
															}
															else
																if (cur_word == "(")
																{
																	skb++;
																	opsk = 1;
																}
																else
																	if (cur_word == "+" || cur_word == "*" || cur_word == "/")
																	{
																		MTable::iterator itr2 = nitr;
																		if ((++itr2) != table.end())
																		{
																			string nxw = itr2->Get_word();
																			if (nxw == ")")
																			{
																				err = 1;
																			}
																			if (sym == 0)
																			{
																				sym++;
																				nm--;
																				opsk = 0;

																			}
																			else
																			{
																				sym++;
																				nm--;
																				err = 1;
																				opsk = 0;

																			}
																		}
																		else
																		{
																			err = 1;
																			break;
																		}
																	}
																	else
																		if (cur_word == "-")
																		{
																			if (opsk == 0)
																			{
																				if (sym > 2)
																					err = 1;
																				else
																				{
																					MTable::iterator itr2 = nitr;
																					if ((++itr2) != table.end())
																					{
																						if (!(itr2->Get_type() == 1 || itr2->Get_type() == 9 || itr2->Get_type() == 10))
																						{
																							string nxw1 = itr2->Get_word();
																							if (nxw1 != "(" && nxw1 != "-")
																								sym++;
																						}
																					}
																					else
																					{
																						err = 1;
																						break;
																					}


																				}
																			}
																			opsk = 0;
																		}
																		else
																			err = 1;
														s_item.rec.s_operand_len++;
														nitr++;
													}
													if (!(skb == 0 && sym == 0 && nm == 1))
														err = 1;
													if (err || ptr_error)
													{
														std::cout << "WAITING FOR PTR OR TOO MANY SYMBOLS in row " << clk << endl;
														string_offset[clk] = string_offset[clk - 1];
													}
													else
													{
														string_offset[clk] = string_offset[clk - 1] + 2;
														opr1.row = clk;
														opr2.row = clk;
														opr2.iconst.is_const = 1;
														opr2.iconst.value = 2;
														vlopr.push_back(opr1);
														vlopr.push_back(opr2);
													}
													itr = --nitr;
												}
												else
												{
													auto it = VSV.begin();
													int is_var_pr = 0;
													string var_name;
													int ptr_error = 0;
													if (cur_word == "BYTE")
													{
														nitr++;
														cur_word = nitr->Get_word();
														if (cur_word == "PTR")
														{
															opr2.ptr.is_ptr = 1;
															opr2.ptr.ptr_type = 2;
														}
														else
															ptr_error = 1;
														nitr++;
														cur_word = nitr->Get_word();
														c_type = nitr->Get_type();
													}
													if (c_type == 7)
													{
														counts++;
														var_name = nitr->Get_word();
														nitr++;
														is_var_pr = 1;
														for (it; it != VSV.end(); it++)
														{
															if (it->name == var_name)
															{
																if (it->type != 1)
																	it = VSV.end();
																break;
															}
														}
													}
													if (!strcmp(nitr->Get_word(), "[") && it != VSV.end())
													{
														int counts2 = 0;
														nitr++;
														if (!get_adr_opr(nitr, opr2, table, &counts2))
														{
															nitr--;
															itr = nitr;
															opr2.undentf.is_undtf = 1;
															opr2.undentf.seg = cur_seg;
															opr2.undentf.type = 1;
															if (is_var_pr)
															{
																opr2.undentf.is_undtf = 1;
																opr2.undentf.seg = cur_seg;
																opr2.undentf.type = 2;
																if (opr2.adr_reg1.adr_num == 3 || opr2.adr_reg2.adr_num == 3)
																{
																	auto it1 = assume.begin();
																	for (it1; it1 != assume.end(); it1++)
																		if (it1->second == it->segment)
																		{
																			if (it1->first != "DS")
																			{
																				string_offset[clk] = string_offset[clk - 1] + 5;
																			}
																			else
																				string_offset[clk] = string_offset[clk - 1] + 4;
																			break;
																		}
																	if (it1 == assume.end() || ptr_error)
																	{
																		string_offset[clk] = string_offset[clk - 1];
																		std::cout << "ERROR! WAITING FOR PTR OF VARIABLE NOT ASSUMED in row " << clk << endl;
																	}
																	else
																	{
																		opr1.row = clk;
																		opr2.row = clk;
																		vlopr.push_back(opr1);
																		vlopr.push_back(opr2);
																	}
																}
																else
																	if (opr2.adr_reg1.adr_num == 5 || opr2.adr_reg2.adr_num == 5)
																	{
																		auto it1 = assume.begin();
																		for (it1; it1 != assume.end(); it1++)
																			if (it1->second == it->segment)
																			{
																				if (it1->first != "SS")
																				{
																					string_offset[clk] = string_offset[clk - 1] + 5;
																				}
																				else
																					string_offset[clk] = string_offset[clk - 1] + 4;
																				break;
																			}
																		if (it1 == assume.end() || ptr_error)
																		{
																			string_offset[clk] = string_offset[clk - 1];
																			std::cout << "ERROR! WAITING FOR PTR OF VARIABLE NOT ASSUMED in row " << clk << endl;
																		}
																		else
																		{
																			opr1.row = clk;
																			opr2.row = clk;
																			vlopr.push_back(opr1);
																			vlopr.push_back(opr2);
																		}
																	}
															}
															else
																string_offset[clk] = string_offset[clk - 1] + 2;
														}
													}
													else
													{
														std::cout << "INVALID ADRESSATION OR NOT CORRECT POP FORM in line " << clk << endl;
														string_offset[clk] = string_offset[clk - 1];
														while (itr != table.end() && itr->Get_string_number() != next_string)
															itr++;
														itr--;
													}
												}
											}
											}
										}
										else
										{
											itr++;
											ind = 4;
											string_offset[clk] = string_offset[clk - 1];
											std::cout << "MOV ERROR in line " << clk <<endl;
											while (itr != table.end() && itr->Get_string_number() < next_string)
											{
												string s1 = itr->Get_word();
												if (s1 == "," && ind == 4)
												{
													ind += 2;
													s_item.rec.s_operand_num = s_item.rec.f_operand_len + s_item.rec.f_operand_num + 1;
												}
												else
													s_item.arr[ind]++;
												itr++;
											}
											itr--;
										}
								}
								else
								{
									if (word == "CALL")
									{
										l_operand opr1;
										l_operand opr2;
										null_l_oprand(opr1);
										null_l_oprand(opr2);
										string c_word = nitr->Get_word();
										size_t c_type = nitr->Get_type();
										opr1.comand_num = 6;
										opr2.comand_num = 6;
										opr1.row = clk;
										opr2.row = clk;
										s_item.rec.mem_code_len = 1;
										s_item.rec.mem_code_num = counts++;
										if (c_type == 22 || c_type == 23)
										{
											get_reg_opr(opr1, c_word);
											if ((++nitr)->Get_string_number() == next_string)
											{
												s_item.rec.f_operand_len = 1;
												s_item.rec.f_operand_num = counts++;
												string_offset[clk] = string_offset[clk - 1] + 2;
												itr++;
											}
											else
											{
												std::cout << "OPERATOR EXPECTED in row " << clk << endl;
												string_offset[clk] = string_offset[clk - 1];
												while (itr != table.end() && itr->Get_string_number() != next_string)
													itr++;
												itr--;
											}
										}
										else
											if (c_word == "FAR")
											{
												string c_word = (++nitr)->Get_word();
												size_t c_type = nitr->Get_type();
												if (c_word == "PTR")
												{
													string c_word = (++nitr)->Get_word();
													size_t c_type = nitr->Get_type();
													if (c_type == 7)
													{
														auto it = PSV.begin();
														for (it ; it != PSV.end(); it++)
														{
															if (c_word == it->pname)
															{
																opr1.comand_num = 5;
																opr1.ptr.is_ptr = 1;
																opr1.ptr.ptr_type = 4;
																opr1.undentf.is_undtf = 1;
																opr1.undentf.type = it->ptype;
																opr1.undentf.seg = it->sname;
																string_offset[clk] = string_offset[clk - 1] + 5;
																vlopr.push_back(opr1);
																vlopr.push_back(opr2);
																break;
															}
														}
														if (it == PSV.end() || (++nitr) == table.end() || nitr->Get_string_number() < next_string)
														{
															string_offset[clk] = string_offset[clk - 1];
															itr = nitr;
															while (itr != table.end() && itr->Get_string_number() != next_string)
																itr++;
															itr--;
														}
														else
														{
															itr = nitr;
															itr--;
														}
													}
													else
													{
														std::cout << "OPERATOR EXPECTED in row " << clk << endl;
														string_offset[clk] = string_offset[clk - 1];
														while (itr != table.end() && itr->Get_string_number() != next_string)
															itr++;
														itr--;
													}
												}
												else
												{
													std::cout << "OPERATOR EXPECTED in row " << clk << endl;
													string_offset[clk] = string_offset[clk - 1];
													while (itr != table.end() && itr->Get_string_number() != next_string)
														itr++;
													itr--;
												}
											}
											else
												if (c_word == "DWORD")
												{
													string c_word = (++nitr)->Get_word();
													size_t c_type = nitr->Get_type();
													if (c_word == "PTR")
													{
														string c_word = (++nitr)->Get_word();
														size_t c_type = nitr->Get_type();
														if (c_type == 7)
														{
															auto it = VSV.begin();
															for (it; it != VSV.end(); it++)
															{
																if (c_word == it->name)
																{
																	opr1.comand_num = 5;
																	opr2.comand_num = 5;
																	opr1.ptr.is_ptr = 1;
																	opr1.ptr.ptr_type = 4;
																	vlopr.push_back(opr1);
																	vlopr.push_back(opr2);
																	string_offset[clk] = string_offset[clk - 1] + 4;
																	break;
																}
															}
															if (it == VSV.end() || (++nitr) == table.end() || nitr->Get_string_number() < next_string)
															{
																string_offset[clk] = string_offset[clk - 1];
																std::cout << "OPERAND TYPE DISMATCH in row " << clk << endl;
																itr = nitr;
																while (itr != table.end() && itr->Get_string_number() != next_string)
																	itr++;
																itr--;
															}
															else
															{
																itr = nitr;
																itr--;
															}
														}
														else
														{
															std::cout << "INVALID CALL PARAMETERS in row " << clk << endl;
															string_offset[clk] = string_offset[clk - 1];
															while (itr != table.end() && itr->Get_string_number() != next_string)
																itr++;
															itr--;
														}
													}
													else
													{
														std::cout << "OPERATOR EXPECTED in row " << clk << endl;
														string_offset[clk] = string_offset[clk - 1];
														while (itr != table.end() && itr->Get_string_number() != next_string)
															itr++;
														itr--;
													}
												}
												else
												{
													if (c_word == "WORD")
													{
														string c_word = (++nitr)->Get_word();
														size_t c_type = nitr->Get_type();
														if (c_word == "PTR")
														{
															string c_word = (++nitr)->Get_word();
															size_t c_type = nitr->Get_type();
															if (c_type == 7)
															{
																auto it = VSV.begin();
																for (it; it != VSV.end(); it++)
																{
																	if (c_word == it->name)
																	{
																		if (it->type > 1)
																			string_offset[clk] = string_offset[clk - 1] + 4;
																		else
																			it = VSV.end();
																		break;
																	}
																}
																if (it == VSV.end() || (++nitr) == table.end() || nitr->Get_string_number() < next_string)
																{
																	string_offset[clk] = string_offset[clk - 1];
																	std::cout << "OPERAND TYPE DISMATCH in row " << clk << endl;
																	itr = nitr;
																	while (itr != table.end() && itr->Get_string_number() != next_string)
																		itr++;
																	itr--;
																}
																else
																{
																	itr = nitr;
																	itr--;
																}
															}
															else
															{
																std::cout << "INVALID CALL PARAMETERS in row " << clk << endl;
																string_offset[clk] = string_offset[clk - 1];
																while (itr != table.end() && itr->Get_string_number() != next_string)
																	itr++;
																itr--;
															}
														}
														else
														{
															std::cout << "OPERATOR EXPECTED in row " << clk << endl;
															string_offset[clk] = string_offset[clk - 1];
															while (itr != table.end() && itr->Get_string_number() != next_string)
																itr++;
															itr--;
														}
													}
													else
													{
														if (c_word == "NEAR")
														{
															string c_word = (++nitr)->Get_word();
															size_t c_type = nitr->Get_type();
															if (c_word == "PTR")
															{
																string c_word = (++nitr)->Get_word();
																size_t c_type = nitr->Get_type();
																if (c_type == 7)
																{
																	auto it = PSV.begin();
																	for (it; it != PSV.end(); it++)
																	{
																		if (c_word == it->pname)
																		{
																			if (it->sname == cur_seg)
																			{
																				opr1.comand_num = 5;
																				opr1.ptr.is_ptr = 1;
																				opr1.ptr.ptr_type = 4;
																				opr1.undentf.is_undtf = 1;
																				opr1.undentf.type = it->ptype;
																				opr1.undentf.seg = it->sname;
																				string_offset[clk] = string_offset[clk - 1] + 5;
																				vlopr.push_back(opr1);
																				vlopr.push_back(opr2);
																			}
																			else
																				it = PSV.end();
																			break;

																		}
																	}
																	if (it == PSV.end() || (++nitr) == table.end() || nitr->Get_string_number() < next_string)
																	{
																		std::cout << "NEAR CALL TO DIFFERENT CS in row " << clk << endl;
																		string_offset[clk] = string_offset[clk - 1];
																		itr = nitr;
																		while (itr != table.end() && itr->Get_string_number() != next_string)
																			itr++;
																		itr--;
																	}
																	else
																	{
																		itr = nitr;
																		itr--;
																	}
																}
																else
																{
																	std::cout << "OPERATOR EXPECTED in row " << clk << endl;
																	string_offset[clk] = string_offset[clk - 1];
																	while (itr != table.end() && itr->Get_string_number() != next_string)
																		itr++;
																	itr--;
																}
															}
															else
															{
																std::cout << "OPERATOR EXPECTED in row " << clk << endl;
																string_offset[clk] = string_offset[clk - 1];
																while (itr != table.end() && itr->Get_string_number() != next_string)
																	itr++;
																itr--;
															}
														}
														else
														{
															if (c_type == 7)
															{
																string c_word = nitr->Get_word();
																auto it = VSV.begin();
																auto it1 = PSV.begin();
																for (it; it != VSV.end(); it++)
																{
																	if (c_word == it->name)
																	{
																		string_offset[clk] = string_offset[clk - 1] + 4;
																		break;
																	}
																}
																for (it1; it1 != PSV.end(); it1++)
																{
																	if (c_word == it1->pname)
																	{
																		if (it1->ptype == 2)
																			string_offset[clk] = string_offset[clk - 1] + 3;
																		else
																			it1 = PSV.end();
																		break;
																	}
																}
																if ((it == VSV.end() && it1 == PSV.end()) || (++nitr) == table.end() || nitr->Get_string_number() < next_string)
																{
																	string_offset[clk] = string_offset[clk - 1];
																	std::cout << "OPERAND TYPE DISMATCH in row " << clk << endl;
																	itr = nitr;
																	while (itr != table.end() && itr->Get_string_number() != next_string)
																		itr++;
																	itr--;
																}
																else
																{
																	itr = nitr;
																	itr--;
																}
															}
															else
															{
																string_offset[clk] = string_offset[clk - 1];
																std::cout << "OPERAND TYPE DISMATCH in row " << clk << endl;
																itr = nitr;
																while (itr != table.end() && itr->Get_string_number() != next_string)
																	itr++;
																itr--;
															}
														}
													}
												}
									}
									else
										if (word == "RET")
										{
											l_operand opr1;
											l_operand opr2;
											null_l_oprand(opr1);
											null_l_oprand(opr2);
											nitr = itr;
											if ((++nitr)->Get_string_number() == next_string)
												string_offset[clk] = string_offset[clk - 1] + 1;
											else
											{
												string_offset[clk] = string_offset[clk - 1];
												std::cout << "OPERAND TYPE DISMATCH in row " << clk << endl;
												itr = nitr;
												while (itr != table.end() && itr->Get_string_number() != next_string)
													itr++;
												itr--;
											}
										}
										else
											if (word == "JNE")
											{
												string c_word;
												size_t c_type;
												nitr = itr;
												c_word = (++nitr)->Get_word();
												c_type = nitr->Get_type();
												s_item.rec.f_operand_num = 2;
												s_item.rec.f_operand_len = 1;
												if (c_type == 7)
												{
													auto it = LSV.begin();
													for (it; it != LSV.end(); it++)
													{
														if (c_word == it->lname)
														{
															if (it->sname == cur_seg)
															{
																string_offset[clk] = string_offset[clk - 1] + 2;

															}
															else
															{
																it = LSV.end();
															}
															break;
														}
													}
													if (it == LSV.end())
													{
														string_offset[clk] = string_offset[clk - 1];
														std::cout << "OPERAND NOT IN CS ASSUME in line "<<clk<<endl;
													}
													else
														if ((++nitr)->Get_string_number() < next_string)
														{
															string_offset[clk] = string_offset[clk - 1];
															std::cout << "TOO MANY ARGUMENTS in line " << clk << endl;
															itr = nitr;
															while (itr != table.end() && itr->Get_string_number() != next_string)
															{
																s_item.rec.f_operand_len++;
																itr++;
															}
														}
														else
															itr++;
												}
												else
												{
													string_offset[clk] = string_offset[clk - 1];
													std::cout << "ERROR JNE in line " << clk << endl;
													while (itr != table.end() && itr->Get_string_number() != next_string)
														itr++;
													itr--;
												}
											}
											else
											{
												string_offset[clk] = string_offset[clk - 1];
												std::cout << "OPERAND TYPE DISMATCH in row " << clk << endl;
												itr = nitr;
												while (itr != table.end() && itr->Get_string_number() != next_string)
													itr++;
												itr--;
											}

								}
							}
						}
					}
				}
			}
		}
		itr++;
		if ((itr == table.end() || itr->Get_string_number() == next_string) && word != "ASSUME")
		{
			ss_tab.push_back(s_item);
			ind = 0;
			counts = 1;
			for (int i = 0; i<6; i++)
				s_item.arr[i]=0;
			if (clk < 10)
			{
				std::cout << "  00" << clk << "  ";
				f << "  00" << clk << "  ";
			}
			else
				if (clk < 100)
				{
					std::cout << "  0" << clk << "  ";
					f << "  0" << clk << "  ";
				}
				else
				{
					std::cout << "  " << clk << "  ";
					f << "  " << clk << "  ";
				}
			std::cout <<"00"<< get16(string_offset[clk-1])<<"	";
			f << "00" << get16(string_offset[clk - 1]) << "	    ";
			while (first_word != table.end() && first_word->Get_string_number() != next_string)
			{
				std::cout << first_word->Get_word() << " ";
				f << first_word->Get_word() << " ";
				first_word++;
			}
			std::cout << endl;
			f << endl;
			if (seg_end)
			{
				std::cout << endl;
				f << endl;
				seg_end = 0;
			}
			clk++;
			first_word = itr;
		}
		else
			if (word == "ASSUME")
			{
				//cout << string_offset[clk - 1] << "	";
				std::cout << "		";
				f << "		";
				while (first_word != table.end() && first_word->Get_string_number() != next_string)
				{
					std::cout << first_word->Get_word() << " ";
					f << first_word->Get_word() << " ";
					first_word++;
				}
				first_word = itr;
				std::cout << endl;
				f << endl;
				clk++;
			}
	}
	ofstream f1;
	f1.open("Operands.txt");
	f1.close();
	itr = table.begin();
	for (int i = 0; i < vlopr.size(); i++)
	{
		f1.open("Operands.txt", ios_base::app);
		while (itr != table.end() && itr->Get_string_number() < vlopr[i].row)
			itr++;
		while (itr != table.end() && itr->Get_string_number() != vlopr[i].row + 1)
		{
			f1 << itr->Get_word() << " ";
			itr++;
		}
		f1 << endl;
		f1.close();
		print_operand(vlopr[i]);
	}
	f.close();
	print_SS_Array(table, ss_tab, V_assume, lvector, vvector, svector, pvector);

	size_t asc = 0;
	std::cout << endl;
	std::cout << endl;
	f1.open("List2.lst");
	f1.close();
	int j = 0;
	itr = table.begin();
	/*
	for (int i = 1; i < clk; i++)
	{
		size_t nxst = itr->Get_string_number() + 1;
		string word = itr->Get_word();
		size_t type = itr->Get_type();
		f.open("List2.lst", ios_base::app);
		if (i < 10)
		{
			std::cout << "  00" << i << "  ";
			f << "  00" << i << "  ";
		}
		else
			if (i < 100)
			{
				std::cout << "  0" << i << "  ";
				f << "  0" << i << "  ";
			}
			else
			{
				std::cout << "  " << i << "  ";
				f << "  " << i << "  ";
			}
		
		for (auto it = svector.begin(); it != svector.end(); it++)
		{
			if (it->name == word)
			{
				type = 100;
				f << "00" << get16(string_offset[i - 1]) << "  ";
				std::cout << "00" << get16(string_offset[i - 1]) << "  ";
				while (itr != table.end() && itr->Get_string_number() < nxst)
				{
					f << itr->Get_word() << " ";
					std::cout<< itr->Get_word() << " ";
					if (!strcmp(itr->Get_word(), "ENDS"))
					{
						f << endl;
						std::cout << endl;
					}
					itr++;
				}
				f << endl;
				std::cout << endl;
				break;
			}
		}
		for (auto it = pvector.begin(); it != pvector.end(); it++)
		{
			if (it->name == word)
			{
				type = 100;
				f << "00" << get16(string_offset[i - 1]) << "  ";
				std::cout << "00" << get16(string_offset[i - 1]) << "  ";
				while (itr != table.end() && itr->Get_string_number() < nxst)
				{
					f << itr->Get_word() << " ";
					std::cout << itr->Get_word() << " ";
					itr++;
				}
				f << endl;
				std::cout << endl;
				break;
			}
		}
		for (auto it = lvector.begin(); it != lvector.end(); it++)
		{
			if (it->name == word)
			{
				type = 100;
				f << "00" << get16(string_offset[i - 1]) << "  ";
				std::cout << "00" << get16(string_offset[i - 1]) << "  ";
				while (itr != table.end() && itr->Get_string_number() < nxst)
				{
					f << itr->Get_word() << " ";
					std::cout << itr->Get_word() << " ";
					itr++;
				}
				f << endl;
				std::cout << endl;
				break;
			}
		}
		for (auto it = vvector.begin(); it != vvector.end(); it++)
		{
			if (it->name == word)
			{
				type = 100;
				f << "00" << get16(string_offset[i - 1]) << "  ";
				std::cout << "00" << get16(string_offset[i - 1]) << "  ";
				if (it->size == 1)
				{
					std::cout << it->value << "        ";
					f << it->value << "        ";
				}
				else
					if (it->size == 2)
					{
						std::cout << "000" << it->value << "      ";
						f << "000"<< it->value << "      ";
					}
					else
						if (it->size == 4)
						{
							std::cout << "0000000" << it->value << "    ";
							f << "0000000" << it->value << "    ";
						}
				while (itr != table.end() && itr->Get_string_number() < nxst)
				{
					f << itr->Get_word() << " ";
					std::cout << itr->Get_word() << " ";
					itr++;
				}
				f << endl;
				std::cout << endl;
				break;
			}
		}
		if (type == 7)
		{
			MTable::iterator nitr = itr;
			string nword = (++nitr)->Get_word();
			if (nword == "EQU")
			{
				std::cout << " =" << (++nitr)->Get_word() << "        ";
				f << " =" << (++nitr)->Get_word() << "        ";
				while (itr != table.end() && itr->Get_string_number() < nxst)
				{
					f << itr->Get_word() << " ";
					std::cout << itr->Get_word() << " ";
					itr++;
				}
				std::cout << endl;
				f << endl;
			}
		}
		else
		{
			if (word == "ASSUME")
			{
				asc++;
				std::cout << "     " ;
				f << "     " ;
				while (itr != table.end() && itr->Get_string_number() < nxst)
				{
					f << itr->Get_word() << " ";
					std::cout << itr->Get_word() << " ";
					itr++;
				}
				std::cout << endl;
				f << endl;
			}
		}
		if (j != vlopr.size())
		{
			if (vlopr[j].row == i)
			{
				f << "00" << get16(string_offset[i - 1]) << "  ";
				std::cout << "00" << get16(string_offset[i - 1]) << "  ";
				f.close();
				get_offset(vlopr[j], vlopr[j + 1], vlopr[j].comand_num, assume, asc);
				f.open("List2.lst", ios_base::app);
				while (itr != table.end() && itr->Get_string_number() < nxst)
				{
					f << itr->Get_word() << " ";
					std::cout << itr->Get_word() << " ";
					itr++;
				}
				f << endl;
				std::cout << endl;
				j += 2;
			}
		}
		f.close();
	}*/
	return 0;
}


int get_adr_opr(MTable::iterator &itr, l_operand &opr, MTable table,int* counts)
{
	size_t nxst = itr->Get_string_number() + 1;
	string word = itr->Get_word();
	int is_err = 1;
	MTable::iterator nitr = itr;
	if (word == "BX")
	{
		opr.adr_reg1.is_adr_reg = 1;
		opr.adr_reg1.adr_bit = 16;
		opr.adr_reg1.adr_num = 3;
		word = (++itr)->Get_word();
		if (word == "+")
		{
			word = (++itr)->Get_word();
			if (word == "SI")
			{
				opr.adr_reg2.is_adr_reg = 1;
				opr.adr_reg2.adr_bit = 16;
				opr.adr_reg2.adr_num = 6;
				word = (++itr)->Get_word();
				if (word == "]" && (++itr)->Get_string_number() == nxst)
				{
					is_err = 0;
					*counts = 4;
					return 0;
				}
			}
			else
				if (word == "DI")
				{
					opr.adr_reg2.is_adr_reg = 1;
					opr.adr_reg2.adr_bit = 16;
					opr.adr_reg2.adr_num = 7;
					word = (++itr)->Get_word();
					if (word == "]" && (++itr)->Get_string_number() == nxst)
					{
						is_err = 0;
						*counts = 4;
						return 0;
					}
				}
		}
	}
	else
		if (word == "BP")
		{
			opr.adr_reg1.is_adr_reg = 1;
			opr.adr_reg1.adr_bit = 16;
			opr.adr_reg1.adr_num = 5;
			word = (++itr)->Get_word();
			if (word == "+")
			{
				word = (++itr)->Get_word();
				if (word == "SI")
				{
					opr.adr_reg2.is_adr_reg = 1;
					opr.adr_reg2.adr_bit = 16;
					opr.adr_reg2.adr_num = 6;
					word = (++itr)->Get_word();
					if (word == "]" && (++itr)->Get_string_number() == nxst)
					{
						is_err = 0;
						*counts = 4;
						return 0;
					}
				}
				else
					if (word == "DI")
					{
						opr.adr_reg2.is_adr_reg = 1;
						opr.adr_reg2.adr_bit = 16;
						opr.adr_reg2.adr_num = 7;
						word = (++itr)->Get_word();
						if (word == "]" && (++itr)->Get_string_number() == nxst)
						{
							is_err = 0;
							*counts = 4;
							return 0;
						}
					}
			}
		}
		else
			if (word == "SI")
			{
				opr.adr_reg1.is_adr_reg = 1;
				opr.adr_reg1.adr_bit = 16;
				opr.adr_reg1.adr_num = 6;
				word = (++itr)->Get_word();
				if (word == "+")
				{
					word = (++itr)->Get_word();
					if (word == "BX")
					{
						opr.adr_reg2.is_adr_reg = 1;
						opr.adr_reg2.adr_bit = 16;
						opr.adr_reg2.adr_num = 3;
						word = (++itr)->Get_word();
						if (word == "]" && (++itr)->Get_string_number() == nxst)
						{
							is_err = 0;
							*counts = 4;
							return 0;
						}
					}
					else
						if (word == "BP")
						{
							opr.adr_reg2.is_adr_reg = 1;
							opr.adr_reg2.adr_bit = 16;
							opr.adr_reg2.adr_num = 5;
							word = (++itr)->Get_word();
							if (word == "]" && (++itr)->Get_string_number() == nxst)
							{
								is_err = 0;
								*counts = 4;
								return 0;
							}
						}
				}
			}
			else
				if (word == "DI")
				{
					opr.adr_reg1.is_adr_reg = 1;
					opr.adr_reg1.adr_bit = 16;
					opr.adr_reg1.adr_num = 7;
					word = (++itr)->Get_word();
					if (word == "+")
					{
						word = (++itr)->Get_word();
						if (word == "BX")
						{
							opr.adr_reg2.is_adr_reg = 1;
							opr.adr_reg2.adr_bit = 16;
							opr.adr_reg2.adr_num = 3;
							word = (++itr)->Get_word();
							if (word == "]" && (++itr)->Get_string_number() == nxst)
							{
								is_err = 0;
								*counts = 4;
								return 0;
							}

						}
						else
							if (word == "BP")
							{
								opr.adr_reg2.is_adr_reg = 1;
								opr.adr_reg2.adr_bit = 16;
								opr.adr_reg2.adr_num = 5;
								word = (++itr)->Get_word();
								if (word == "]" && (++itr)->Get_string_number() == nxst)
									if (word == "]" && (++itr)->Get_string_number() == nxst)
									{
										*counts = 4;
										itr--;
										return 0;
									}
							}
					}
				}
	while (nitr != table.end() && nitr->Get_string_number() < nxst)
	{
			opr.adr_reg1.is_adr_reg = 0;
			opr.adr_reg1.adr_bit = 0;
			opr.adr_reg1.adr_num = 0;
			opr.adr_reg2.is_adr_reg = 0;
			opr.adr_reg2.adr_bit = 0;
			opr.adr_reg2.adr_num = 0;
			nitr++;
			counts++;
	}
	itr == nitr;
	itr--;
	return 1;
}

void null_l_oprand(l_operand& opr)
{
	opr.adr_reg1.adr_bit = 0;
	opr.adr_reg1.adr_num = 0;
	opr.adr_reg1.is_adr_reg = 0;
	opr.adr_reg2.adr_bit = 0;
	opr.adr_reg2.adr_num = 0;
	opr.adr_reg2.is_adr_reg = 0;
	opr.comand_num = 0;
	opr.iconst.is_const = 0;
	opr.iconst.value = 0;
	opr.is_adr_com = 0;
	opr.label.is_label = 0;
	opr.label.l_str_num = 0;
	opr.mult.adr_reg_num = 0;
	opr.mult.is_mult = 0;
	opr.mult.value = 0;
	opr.pref.is_pref = 0;
	opr.pref.seg_num = 0;
	opr.ptr.is_ptr = 0;
	opr.ptr.ptr_type = 0;
	opr.reg.is_reg = 0;
	opr.reg.r_bit = 0;
	opr.reg.r_num = 0;
	opr.row = 0;
	opr.undentf.is_undtf = 0;
	opr.undentf.offset = 0;
	opr.undentf.seg = "-";
	opr.undentf.type = 0;
}

void print_operand(l_operand opr)
{
	ofstream f;
	f.open("Operands.txt", ios_base::app);
	f << "Наявність регістра даних	             | " << opr.reg.is_reg << " | Розрядність    	                                 | " << opr.reg.r_bit << "       | Номер регістра даних | " << opr.reg.r_num << endl;
	f << "Наявність оператора ptr                  | " << opr.ptr.is_ptr << " | Тип                                               | " << opr.ptr.ptr_type << endl;
	f << "Наявність префікса замені сегмента       | " << opr.pref.is_pref << " | Номер сегментного регістра                        | " << opr.pref.seg_num << endl;
	f << "Наявність ідентифікатора мітки або імені | " << opr.label.is_label << " | Номер рядка в таблиці ідентифікаторів користувача | " << opr.label.l_str_num << endl;
	f << "Наявність невізначенго ідентифікатора    | " << opr.undentf.is_undtf << " | Сегмент                                           | " << opr.undentf.seg << "       | Тип                  | " << opr.undentf.type << endl;
	f << "Наявність адресного регістра 1           | " << opr.adr_reg1.is_adr_reg << " | Номер регістра                                    | " << opr.adr_reg1.adr_num << "       | Розрядність          | " << opr.adr_reg1.adr_bit << endl;
	f << "Наявність адресного регістра 2           | " << opr.adr_reg2.is_adr_reg << " | Номер регістра                                    | " << opr.adr_reg2.adr_num << "       | Розрядність          | " << opr.adr_reg2.adr_bit << endl;
	f << "Наявність множника                       | " << opr.mult.is_mult << " | Номер регістра адрес який має множник             | " << opr.mult.adr_reg_num << "       | Значення множника    | " << opr.mult.value << endl;
	f << "Наявність операторів offset або seg      | " << opr.is_adr_com << " |" << endl;
	f << "Наявність констант                       | " << opr.iconst.is_const << " | Значення       	                                 | " << opr.iconst.value << endl;
	f << "Номер команди                            | " << opr.comand_num << " |"<<endl;
	f << "Номер рядка                              | " << opr.row<< "|" << endl;
	f << endl;
	f.close();
}

int get_offset(l_operand opr1, l_operand opr2, int comand_num, ASSUME_TABLE assume, int call_stack)
{
	ofstream f;
	int offset = 0;
	f.open("List2.lst", ios_base::app);
	if (comand_num == 1)
	{
		if (call_stack == 1)
			f << "C3					";
		else
			f << "CB					";
		f.close();
		return 1;
	}
	if (comand_num == 2) // PUSH
	{
		if (!opr1.ptr.is_ptr && !opr1.pref.is_pref && !opr1.label.is_label && !opr1.undentf.is_undtf && !opr1.mult.is_mult
			&& !opr1.is_adr_com && !opr1.iconst.is_const && (opr1.reg.is_reg || (opr1.adr_reg1.is_adr_reg && !opr1.adr_reg2.is_adr_reg)))
		{
			if (opr1.reg.r_bit == 16 || opr1.adr_reg1.is_adr_reg)
			{
				int rnum;
				char res[5] = { 0 };
				if (opr1.adr_reg1.is_adr_reg)
					rnum = opr1.adr_reg1.adr_num;
				else
					rnum = opr1.reg.r_num;
				strcpy_s(res, 5, get16(rnum));
				f << "5" << res[1] << "					";
				f.close();
				return 1;
			}
		}
		return 0;
	}
	if (comand_num == 3)
	{
		if (!opr1.reg.is_reg && !opr1.ptr.is_ptr && !opr1.pref.is_pref && !opr1.label.is_label && !opr1.undentf.is_undtf && opr1.adr_reg1.is_adr_reg
			&& opr1.adr_reg2.is_adr_reg && !opr1.mult.is_mult && !opr1.is_adr_com && !opr1.iconst.is_const)
		{
			if ((opr1.adr_reg1.adr_num == 3 && opr1.adr_reg2.adr_num == 6) || (opr1.adr_reg1.adr_num == 6 && opr1.adr_reg2.adr_num == 3))
				f << "8F 00			    ";
			else
				if ((opr1.adr_reg1.adr_num == 3 && opr1.adr_reg2.adr_num == 7) || (opr1.adr_reg1.adr_num == 7 && opr1.adr_reg2.adr_num == 3))
					f << "8F 01			    ";
				else
					if ((opr1.adr_reg1.adr_num == 5 && opr1.adr_reg2.adr_num == 6) || (opr1.adr_reg1.adr_num == 6 && opr1.adr_reg2.adr_num == 5))
						f << "8F 02			    ";
					else
						if ((opr1.adr_reg1.adr_num == 5 && opr1.adr_reg2.adr_num == 7) || (opr1.adr_reg1.adr_num == 7 && opr1.adr_reg2.adr_num == 5))
							f << "8F 03			    ";
			f << "	";
			f.close();
			return 1;
		}
		else
		{
			if (!opr1.reg.is_reg && !opr1.ptr.is_ptr && !opr1.pref.is_pref && !opr1.label.is_label && opr1.undentf.is_undtf && opr1.adr_reg1.is_adr_reg
				&& opr1.adr_reg2.is_adr_reg && !opr1.mult.is_mult && !opr1.is_adr_com && !opr1.iconst.is_const)
			{
				auto mit = assume.begin();
				if ((opr1.adr_reg1.adr_num == 3 && opr1.adr_reg2.adr_num == 6) || (opr1.adr_reg1.adr_num == 6 && opr1.adr_reg2.adr_num == 3))
				{
					for (mit; mit != assume.end(); mit++)
					{
						if (mit->second == opr1.undentf.seg)
						{
							string seg = mit->first;
							if (seg == "ES")
								f << "26:";
							else
								if (seg == "CS")
									f << "2E:";
								else
									if (seg == "SS")
										f << "36:";
									else
										if (seg == "FS")
											f << "64:";
										else
											if (seg == "GS")
												f << "65:";
							break;

						}
					}
					f << "8F 80	";
				}
				else
					if ((opr1.adr_reg1.adr_num == 3 && opr1.adr_reg2.adr_num == 7) || (opr1.adr_reg1.adr_num == 7 && opr1.adr_reg2.adr_num == 3))
					{
						for (mit; mit != assume.end(); mit++)
						{
							if (mit->second == opr1.undentf.seg)
							{
								string seg = mit->first;
								if (seg == "ES")
									f << "26:";
								else
									if (seg == "CS")
										f << "2E:";
									else
										if (seg == "SS")
											f << "36:";
										else
											if (seg == "FS")
												f << "64:";
											else
												if (seg == "GS")
													f << "65:";
								break;
							}
						}
						f << "8F 81	";
					}
					else
						if ((opr1.adr_reg1.adr_num == 5 && opr1.adr_reg2.adr_num == 6) || (opr1.adr_reg1.adr_num == 6 && opr1.adr_reg2.adr_num == 5))
						{
							for (mit; mit != assume.end(); mit++)
							{
								if (mit->second == opr1.undentf.seg)
								{
									string seg = mit->first;
									if (seg == "ES")
										f << "26:";
									else
										if (seg == "CS")
											f << "2E:";
										else
											if (seg == "FS")
												f << "64:";
											else
												if (seg == "GS")
													f << "65:";
												else
													if (seg == "DS")
														f << "3E:";
									break;
								}
							}
							f << "8F 82	";
						}
						else
							if ((opr1.adr_reg1.adr_num == 5 && opr1.adr_reg2.adr_num == 7) || (opr1.adr_reg1.adr_num == 7 && opr1.adr_reg2.adr_num == 5))
							{
								for (mit; mit != assume.end(); mit++)
								{
									if (mit->second == opr1.undentf.seg)
									{
										string seg = mit->first;
										if (seg == "ES")
											f << "26:";
										else
											if (seg == "CS")
												f << "2E:";
											else
												if (seg == "FS")
													f << "64:";
												else
													if (seg == "GS")
														f << "65:";
													else
														if (seg == "DS")
															f << "3E:";
										break;
									}
								}
								f << "8F 83	";
							}
				f << "00" << get16(opr1.undentf.offset) << " R	    ";
			}
			f.close();
			return 5;
		}
	}
	if (comand_num == 4)
	{
		if (((opr1.adr_reg1.is_adr_reg && !opr1.adr_reg2.is_adr_reg) || (opr1.reg.is_reg && !opr1.adr_reg1.is_adr_reg)) && !opr1.iconst.is_const
			&& !opr1.is_adr_com && !opr1.label.is_label && !opr1.mult.is_mult && !opr1.pref.is_pref && !opr1.ptr.is_ptr && !opr1.undentf.is_undtf)
		{

			if (!opr2.reg.is_reg && !opr2.adr_reg1.is_adr_reg && !opr2.adr_reg2.is_adr_reg && !opr2.iconst.is_const && opr2.is_adr_com
				&& !opr2.mult.is_mult && !opr2.pref.is_pref && !opr2.ptr.is_ptr)
			{
				if (opr1.adr_reg1.is_adr_reg || opr1.reg.r_bit == 16)
				{
					int rnum = 8;
					int voffset;
					char res[5] = { 0 };
					if (opr2.undentf.is_undtf)
						voffset = opr2.undentf.offset;
					else
						if (opr2.label.is_label)
							voffset = opr2.label.l_str_num;
					if (opr1.adr_reg1.is_adr_reg)
						rnum += opr1.adr_reg1.adr_num;
					else
						rnum += opr1.reg.r_num;

					strcpy_s(res, 5, get16(rnum));
					f << "B" << res[1] << " 00" << get16(voffset) << " R				";
					offset += 3;
					return offset;
				}
				else
					if (opr1.reg.r_bit == 8)
					{
						int voffset;
						int rnum = opr1.reg.r_num;
						if (opr2.undentf.is_undtf)
							voffset = opr2.undentf.offset;
						else
							if (opr2.label.is_label)
								voffset = opr2.label.l_str_num;
						char res[5] = { 0 };
						strcpy_s(res, 5, get16(rnum));
						f << "B" << res[1] << " " << get16(voffset) << " 				";
						offset += 2;
						return offset;
					}
			}
			else
			{
				if (!opr2.reg.is_reg && opr2.adr_reg1.is_adr_reg && opr2.adr_reg2.is_adr_reg && !opr2.iconst.is_const && !opr2.is_adr_com && !opr2.label.is_label
					&& !opr2.mult.is_mult && !opr2.pref.is_pref && !opr2.ptr.is_ptr)
				{
					if (!opr2.undentf.is_undtf)
					{
						if (opr1.reg.r_bit == 16 || (opr1.adr_reg1.is_adr_reg && !opr1.adr_reg2.is_adr_reg))
						{
							int rnum = 0;
							if (opr1.adr_reg1.is_adr_reg)
								rnum = opr1.adr_reg1.adr_num * 8;
							else
								rnum = opr1.reg.r_num * 8;
							offset += 2;
							if ((opr2.adr_reg1.adr_num == 3 && opr2.adr_reg2.adr_num == 6) || (opr2.adr_reg1.adr_num == 6 && opr2.adr_reg2.adr_num == 3))
								rnum += 0;
							else
								if ((opr2.adr_reg1.adr_num == 3 && opr2.adr_reg2.adr_num == 7) || (opr2.adr_reg1.adr_num == 7 && opr2.adr_reg2.adr_num == 3))
									rnum += 1;
								else
									if ((opr2.adr_reg1.adr_num == 5 && opr2.adr_reg2.adr_num == 6) || (opr2.adr_reg1.adr_num == 6 && opr2.adr_reg2.adr_num == 5))
										rnum += 2;
									else
										if ((opr2.adr_reg1.adr_num == 5 && opr2.adr_reg2.adr_num == 7) || (opr2.adr_reg1.adr_num == 7 && opr2.adr_reg2.adr_num == 5))
											rnum += 3;
							f << "8B " << get16(rnum) << "					";
							f.close();
							return offset;
						}
						else
						{
							int rnum = opr1.reg.r_num * 8;
							if ((opr2.adr_reg1.adr_num == 3 && opr2.adr_reg2.adr_num == 6) || (opr2.adr_reg1.adr_num == 6 && opr2.adr_reg2.adr_num == 3))
								rnum += 0;
							else
								if ((opr2.adr_reg1.adr_num == 3 && opr2.adr_reg2.adr_num == 7) || (opr2.adr_reg1.adr_num == 7 && opr2.adr_reg2.adr_num == 3))
									rnum += 1;
								else
									if ((opr2.adr_reg1.adr_num == 5 && opr2.adr_reg2.adr_num == 6) || (opr2.adr_reg1.adr_num == 6 && opr2.adr_reg2.adr_num == 5))
										rnum += 2;
									else
										if ((opr2.adr_reg1.adr_num == 5 && opr2.adr_reg2.adr_num == 7) || (opr2.adr_reg1.adr_num == 7 && opr2.adr_reg2.adr_num == 5))
											rnum += 3;
							f << "8A " << get16(rnum) << "					";
						}
						offset += 2;
						f.close();
						return offset;
					}
					else
					{
						if (opr1.reg.r_bit == 16 || (opr1.adr_reg1.is_adr_reg && !opr1.adr_reg2.is_adr_reg))
						{
							auto mit = assume.begin();
							int rnum = 128;
							if (opr1.adr_reg1.is_adr_reg)
								rnum += opr1.adr_reg1.adr_num * 8;
							else
								rnum += opr1.reg.r_num * 8;
							offset += 4;
							if ((opr2.adr_reg1.adr_num == 3 && opr2.adr_reg2.adr_num == 6) || (opr2.adr_reg1.adr_num == 6 && opr2.adr_reg2.adr_num == 3))
							{
								for (mit; mit != assume.end(); mit++)
								{
									if (mit->second == opr2.undentf.seg)
									{
										offset++;
										string seg = mit->first;
										if (seg == "ES")
											f << "26: ";
										else
											if (seg == "CS")
												f << "2E: ";
											else
												if (seg == "SS")
													f << "36: ";
												else
													if (seg == "FS")
														f << "64: ";
													else
														if (seg == "GS")
															f << "65: ";
														else
															if (seg == "DS ")
																offset--;
										break;
									}
								}
							}
							else
								if ((opr2.adr_reg1.adr_num == 3 && opr2.adr_reg2.adr_num == 7) || (opr2.adr_reg1.adr_num == 7 && opr2.adr_reg2.adr_num == 3))
								{
									rnum++;
									for (mit; mit != assume.end(); mit++)
									{
										if (mit->second == opr2.undentf.seg)
										{
											offset++;
											string seg = mit->first;
											if (seg == "ES")
												f << "26: ";
											else
												if (seg == "CS")
													f << "2E: ";
												else
													if (seg == "SS")
														f << "36: ";
													else
														if (seg == "FS")
															f << "64: ";
														else
															if (seg == "GS")
																f << "65: ";
															else
																if (seg == "DS")
																	offset--;
											break;

										}
									}
								}
								else
									if ((opr2.adr_reg1.adr_num == 5 && opr2.adr_reg2.adr_num == 6) || (opr2.adr_reg1.adr_num == 6 && opr2.adr_reg2.adr_num == 5))
									{
										rnum += 2;
										for (mit; mit != assume.end(); mit++)
										{
											if (mit->second == opr2.undentf.seg)
											{
												offset++;
												string seg = mit->first;
												if (seg == "ES")
													f << "26: ";
												else
													if (seg == "CS")
														f << "2E: ";
													else
														if (seg == "SS")
															offset--;
														else
															if (seg == "FS")
																f << "64: ";
															else
																if (seg == "GS")
																	f << "65: ";
																else
																	if (seg == "DS")
																		f << "3E: ";
												break;
											}
										}
									}
									else
										if ((opr2.adr_reg1.adr_num == 5 && opr2.adr_reg2.adr_num == 7) || (opr2.adr_reg1.adr_num == 7 && opr2.adr_reg2.adr_num == 5))
										{
											rnum += 3;
											for (mit; mit != assume.end(); mit++)
											{
												if (mit->second == opr2.undentf.seg)
												{
													offset++;
													string seg = mit->first;
													if (seg == "ES")
														f << "26:";
													else
														if (seg == "CS")
															f << "2E: ";
														else
															if (seg == "SS")
																offset--;
															else
																if (seg == "FS")
																	f << "64: ";
																else
																	if (seg == "GS")
																		f << "65: ";
																	else
																		if (seg == "DS")
																			f << " 3E: ";
													break;

												}
											}
										}
							f << "8B " << get16(rnum) << " 00" << get16(opr2.undentf.offset) << " R		";
							f.close();
							return offset;
						}
						else
							if (opr1.reg.r_bit == 8)
							{
								auto mit = assume.begin();
								int rnum = 128;
								rnum += opr1.reg.r_num * 8;
								offset += 4;
								if ((opr2.adr_reg1.adr_num == 3 && opr2.adr_reg2.adr_num == 6) || (opr2.adr_reg1.adr_num == 6 && opr2.adr_reg2.adr_num == 3))
								{
									for (mit; mit != assume.end(); mit++)
									{
										if (mit->second == opr2.undentf.seg)
										{
											offset++;
											string seg = mit->first;
											if (seg == "ES")
												f << "26: ";
											else
												if (seg == "CS")
													f << "2E: ";
												else
													if (seg == "SS")
														f << "36: ";
													else
														if (seg == "FS")
															f << "64: ";
														else
															if (seg == "GS")
																f << "65: ";
															else
																if (seg == "DS ")
																	offset--;
											break;
										}
									}
								}
								else
									if ((opr2.adr_reg1.adr_num == 3 && opr2.adr_reg2.adr_num == 7) || (opr2.adr_reg1.adr_num == 7 && opr2.adr_reg2.adr_num == 3))
									{
										rnum++;
										for (mit; mit != assume.end(); mit++)
										{
											if (mit->second == opr2.undentf.seg)
											{
												offset++;
												string seg = mit->first;
												if (seg == "ES")
													f << "26: ";
												else
													if (seg == "CS")
														f << "2E: ";
													else
														if (seg == "SS")
															f << "36: ";
														else
															if (seg == "FS")
																f << "64: ";
															else
																if (seg == "GS")
																	f << "65: ";
																else
																	if (seg == "DS")
																		offset--;
												break;

											}
										}
									}
									else
										if ((opr2.adr_reg1.adr_num == 5 && opr2.adr_reg2.adr_num == 6) || (opr2.adr_reg1.adr_num == 6 && opr2.adr_reg2.adr_num == 5))
										{
											rnum += 2;
											for (mit; mit != assume.end(); mit++)
											{
												if (mit->second == opr2.undentf.seg)
												{
													offset++;
													string seg = mit->first;
													if (seg == "ES")
														f << "26: ";
													else
														if (seg == "CS")
															f << "2E: ";
														else
															if (seg == "SS")
																offset--;
															else
																if (seg == "FS")
																	f << "64: ";
																else
																	if (seg == "GS")
																		f << "65: ";
																	else
																		if (seg == "DS")
																			f << "3E: ";
													break;
												}
											}
										}
										else
											if ((opr2.adr_reg1.adr_num == 5 && opr2.adr_reg2.adr_num == 7) || (opr2.adr_reg1.adr_num == 7 && opr2.adr_reg2.adr_num == 5))
											{
												rnum += 3;
												for (mit; mit != assume.end(); mit++)
												{
													if (mit->second == opr2.undentf.seg)
													{
														offset++;
														string seg = mit->first;
														if (seg == "ES")
															f << "26: ";
														else
															if (seg == "CS")
																f << "2E: ";
															else
																if (seg == "SS")
																	offset--;
																else
																	if (seg == "FS")
																		f << "64: ";
																	else
																		if (seg == "GS")
																			f << "65: ";
																		else
																			if (seg == "DS")
																				f << "3E: ";
														break;

													}
												}
											}
								f << "8A " << get16(rnum) << " 00" << get16(opr2.undentf.offset) << " R		";
								f.close();
								return offset;
							}
					}
				}
				else
				{
					if (!opr2.reg.is_reg && !opr2.adr_reg1.is_adr_reg && !opr2.adr_reg2.is_adr_reg && opr2.iconst.is_const && !opr2.is_adr_com && !opr2.label.is_label
						&& !opr2.mult.is_mult && !opr2.pref.is_pref && !opr2.ptr.is_ptr)
					{
						if (opr1.reg.r_bit == 16 || (opr1.adr_reg1.is_adr_reg && !opr1.adr_reg2.is_adr_reg))
						{
							int rnum = 8;
							char res[5] = { 0 };
							if (opr1.adr_reg1.is_adr_reg)
								rnum += opr1.adr_reg1.adr_num;
							else
								rnum += opr1.reg.r_num;
							strcpy_s(res, 5, get16(rnum));
							f << "B" << res[1] << " 00" << get16(opr2.iconst.value) << " 				";
							f.close();
							offset = 3;
							return offset;
						}
						else
							if (opr1.reg.r_bit == 8)
							{
								int rnum = opr1.reg.r_num;
								char res[5] = { 0 };
								strcpy_s(res, 5, get16(rnum));
								f << "B" << res[1] << " " << get16(opr2.iconst.value) << " 				";
								offset = 2;
								f.close();
								return offset;
							}
					}
				}
			}
		}
	}
	if (comand_num == 6)
	{
		if (!opr1.adr_reg1.is_adr_reg && !opr1.iconst.is_const && !opr1.is_adr_com && !opr1.label.is_label && !opr1.mult.is_mult && !opr1.pref.is_pref
			&& !opr1.reg.is_reg && opr1.undentf.is_undtf)
		{
			if (!opr1.ptr.is_ptr || opr1.ptr.ptr_type == 2)
			{
				f << "E8 00" << get16(opr1.undentf.offset) << " R				";
				f.close();
				return 3;
			}
			else
				if (opr1.ptr.ptr_type == 4)
				{
					f << "9A 00" << get16(opr1.undentf.offset) << " ---- R		";
					f.close();
					return 5;
				}
		}
		else
		{
			if ((opr1.adr_reg1.is_adr_reg || opr1.reg.is_reg) && !opr1.iconst.is_const && !opr1.is_adr_com && !opr1.label.is_label && !opr1.mult.is_mult && !opr1.pref.is_pref
				&& !opr1.undentf.is_undtf && !opr1.ptr.is_ptr)
			{
				int rnum = 208;
				if (opr1.adr_reg1.is_adr_reg)
					rnum += opr1.adr_reg1.adr_num;
				else
					rnum += opr1.reg.r_num;
				f << "FF " << get16(rnum) << "					";
				f.close();
				return 2;
			}
		}
	}
	return 0;
}
