
#include "stdafx.h"





int struct_check(MTable table, SS_table& ss_tab, Assume_vector& V_assume, Label_vector& lvector, Var_vector& vvector, Segment_vector& svector, Proc_vector& pvector)
{
	/*ofstream f;
	MTable::iterator itr = table.end();
	int in_segment = 0;
	int proc_count = 0;
	int clk = 0;
	int indx = 0;
	size_t *string_offset;
	ASSUME_TABLE assume;
	size_t offs;
	T_segments curseg;
	T_proc nproc;
	size_t max_str = (--itr)->Get_string_number();
	int ascount = 0;
	f.open("List2.lst");
	f.close();
	f.open("Operands.txt");
	f.close();
	string_offset = new size_t[max_str];
	offs = 0;
	assume["CS"] = "nothing";
	assume["DS"] = "nothing";
	assume["ES"] = "nothing";
	assume["FS"] = "nothing";
	assume["GS"] = "nothing";
	assume["SS"] = "nothing";
	clk = 1;
	for (itr = table.begin(); itr != table.end(); clk++)
	{
		size_t save;
		MTable::iterator itrout = itr;
		MTable::iterator chk = itr;
		MTable::iterator itr1 = itr;
		SS_item s_item = { 0 };
		size_t next_string = itr->Get_string_number() + 1;
		int ind = 0;
		int num = 1;
		int df = 0;
		int counts = 0;
		int is_wait_val = 0;
		int is_wait_oper = 0;
		int is_end = 0;
		size_t proc_string = 0;
		size_t error_row = 0;
		int is_create_opr = 0;
		l_operand opr1 = { 0 };
		l_operand opr2 = { 0 };
		int is_1_ready = 0;
		string mac_com;
		int is_adr = 0;
		int is_comand = 0;
		int comand_num;
		while (chk != table.end() && chk->Get_string_number() != next_string)
		{
			if (chk->Get_type() == 5)
			{
				is_comand = 1;
				break;
			}
			else
				chk++;
		}
		if (!strcmp(itr->Get_word(), "ASSUME"))
		{
			MTable::iterator asu = itr;
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
				ascount++;
				V_assume.push_back(assume);
				/*assume["CS"] = "nothing";
				assume["DS"] = "nothing";
				assume["ES"] = "nothing";
				assume["FS"] = "nothing";
				assume["GS"] = "nothing";
				assume["SS"] = "nothing";
			}
			else
			{
				std::cout << "ASSUME ERROR 1 " << clk << endl;
			}
			string_offset[offs + 1] = string_offset[offs];
			offs++;
		}
		else
		{
			while (itr != table.end() && itr->Get_string_number() < next_string)
			{
				int type = itr->Get_type();
				MTable::iterator nx = itr;
				T_Variables nvar;
				df = 0;
				if ((++nx) != table.end())
				{
					MTable::iterator nx1 = itr;
					if (ind == 0 && type == 7)
					{
						if (!strcmp(nx->Get_word(), "SEGMENT") || !strcmp(nx->Get_word(), "PROC") || ((proc_count > 0 && !strcmp(nx->Get_word(), "ENDP")) ||
							((!strcmp(nx->Get_word(), ":") || (nx->Get_type() == 6 || !strcmp(nx->Get_word(), "EQU") || !strcmp(nx->Get_word(), "ENDS")) && in_segment > 0))))
						{
							if (!strcmp(nx->Get_word(), ":"))
							{
								Label_vector::iterator litr;
								T_Lables lab;
								lab.name = itr->Get_word();
								for (litr = lvector.begin(); litr != lvector.end(); litr++)
									if (litr->name == lab.name)
									{
										std::cout << "Label in row " << clk << endl;
										break;
									}
								if (litr == lvector.end())
								{
									lab.segment = curseg.name;
									lab.offset = string_offset[offs];
									lvector.push_back(lab);
								}

								string_offset[offs + 1] = string_offset[offs];
							}
							if (!strcmp(nx->Get_word(), "SEGMENT"))
							{
								Segment_vector::iterator sitr;
								curseg.name = itr->Get_word();
								for (sitr = svector.begin(); sitr != svector.end(); sitr++)
									if (sitr->name == curseg.name)
									{
										std::cout << "Segment in row " << clk << endl;
										break;
									}
								string_offset[offs] = 0;
								string_offset[offs + 1] = 0;
							}
							if (!strcmp(nx->Get_word(), "PROC"))
							{
								Proc_vector::iterator pitr;
								nproc.name = itr->Get_word();
								for (pitr = pvector.begin(); pitr != pvector.end(); pitr++)
									if (pitr->name == curseg.name)
									{
										std::cout << "Proc in row " << clk << endl;
										break;
									}
								if (pitr == pvector.end())
								{
									nproc.segment = curseg.name;
									nproc.offset = string_offset[offs];
									pvector.push_back(nproc);
								}
							}
							s_item.rec.label_number = 1;
							ind++;
							num = 2;
						}
						else
						{
							std::cout << "ERROR! " << clk << endl;
							error_row = clk;
						}
					}
					else
					{
						if (ind == 0) ind++;
						// segment 
						if (s_item.rec.label_number == 1 && ind == 1 && !strcmp(itr->Get_word(), "SEGMENT") && nx->Get_string_number() == next_string)
						{
							counts++;
							in_segment++;
						}
						else
							// variable intialization
							if (s_item.rec.label_number == 1 && ind == 1 && (type == 6 || !strcmp(itr->Get_word(), "EQU"))) // set type db dw dd equ
							{
								int clon = 0;
								MTable::iterator varitr = itr;
								Var_vector::iterator vecitr;
								s_item.rec.mem_code_num = 2;
								s_item.rec.mem_code_len = 1;
								ind += 2;
								num++;
								is_wait_val = 1;
								counts = 0;
								// write into var table
								if (!strcmp(itr->Get_word(), "DB"))
								{

									nvar.name = (--varitr)->Get_word();
									for (vecitr = vvector.begin(); vecitr != vvector.end(); vecitr++)
										if (vecitr->name == nvar.name)
										{
											std::cout << "Variable in row " << clk << endl;
											break;
											clon = 1;
										}
									if (vecitr == vvector.end())
									{
										nvar.size = 1;
										nvar.value = 0;
										nvar.segment = curseg.name;
										vvector.push_back(nvar);
									}
									string_offset[offs + 1] = string_offset[offs] + 1;
								}
								else
									if (!strcmp(itr->Get_word(), "DW"))
									{
										nvar.name = (--varitr)->Get_word();
										for (vecitr = vvector.begin(); vecitr != vvector.end(); vecitr++)
											if (vecitr->name == nvar.name)
											{
												std::cout << "Variable in row " << clk << endl;
												break;
												clon = 1;
											}
										if (vecitr == vvector.end())
										{
											nvar.size = 2;
											nvar.value = 0;
											nvar.segment = curseg.name;
											vvector.push_back(nvar);
										}
										string_offset[offs + 1] = string_offset[offs] + 2;
									}
									else
										if (!strcmp(itr->Get_word(), "DD"))
										{
											nvar.name = (--varitr)->Get_word();
											for (vecitr = vvector.begin(); vecitr != vvector.end(); vecitr++)
												if (vecitr->name == nvar.name)
												{
													std::cout << "Variable in row " << clk << endl;
													break;
													clon = 1;
												}
											if (vecitr == vvector.end())
											{
												nvar.size = 4;
												nvar.value = 0;
												nvar.segment = curseg.name;
												vvector.push_back(nvar);
											}
											string_offset[offs + 1] = string_offset[offs] + 4;
										}
										else
											string_offset[offs + 1] = string_offset[offs];
							}
							else
								// variable intialization
								if (ind == 3 && is_wait_val == 1 && s_item.rec.label_number == 1 && (type == 1 || type == 7 || type == 8 || type == 9 || type == 10 || type == 11))
								{
									if (nx->Get_string_number() == next_string)
										is_wait_val = 0;
									counts++;
									// check value and write into var table

								}
								else
									// machine instruction
									if (ind == 1 && type == 5 && (in_segment > 0 || proc_count > 0) && (num == 1 || num == 3))
									{
										s_item.rec.mem_code_num = num;
										s_item.rec.mem_code_len = 1;
										ind += 2;
										num++;
										counts = 0;
										mac_com = itr->Get_word();
										if (!strcmp(itr->Get_word(), "RET"))
										{
											comand_num = 1;
											df = 1; // dont write in the end
											if ((++itr)->Get_string_number() != next_string)
											{
												std::cout << "ERROR, too many arguments in " << next_string << endl;
											}
											else
												itr--;
											string_offset[offs + 1] = string_offset[offs] + 1;

										}
										else
										{
											string_offset[offs + 1] = string_offset[offs];
											is_create_opr = 1;
											MTable::iterator check_arg = itr;
											if ((++check_arg)->Get_string_number() == next_string)
											{
												df = 1;
												std::cout << "ERROR, not enough arguments in " << next_string << endl;
											}
											else
											{
												if (!strcmp(itr->Get_word(), "PUSH") || !strcmp(itr->Get_word(), "POP") || !strcmp(itr->Get_word(), "JNE") || !strcmp(itr->Get_word(), "CALL"))
												{
													check_arg = itr;
													int erflag = 0;
													while (check_arg->Get_string_number() < next_string && check_arg != table.end())
													{
														if (!strcmp(check_arg->Get_word(), ","))
														{
															erflag = 1;
															std::cout << "ERROR, too many arguments in " << next_string << endl;
														}
														check_arg++;
														if (!strcmp(itr->Get_word(), "PUSH"))
															comand_num = 2;
														else
															if (!strcmp(itr->Get_word(), "POP"))
																comand_num = 3;
															else
																if (!strcmp(itr->Get_word(), "JNE"))
																	comand_num = 5;
																else
																	comand_num = 6;
													}
													if (erflag)
													{
														df = 1;
														itr = --check_arg;
													}
												}
												else
													comand_num = 4;
											}
										}
									}
									else
										// registers 
										if ((type == 21 || type == 22 || type == 23) && ind > 2)
										{
											
											if (!strcmp(itr->Get_word(), "AL") && is_adr == 0)
											{
												if (!is_1_ready)
												{
													opr1.reg.is_reg = 1;
													opr1.reg.r_bit = 8;
													opr1.reg.r_num = 0;
												}
												else
												{
													opr2.reg.is_reg = 1;
													opr2.reg.r_bit = 8;
													opr2.reg.r_num = 0;
												}
											}
											else
												if (!strcmp(itr->Get_word(), "CL") && is_adr == 0)
												{
													if (!is_1_ready)
													{
														opr1.reg.is_reg = 1;
														opr1.reg.r_bit = 8;
														opr1.reg.r_num = 1;
													}
													else
													{
														opr2.reg.is_reg = 1;
														opr2.reg.r_bit = 8;
														opr2.reg.r_num = 1;
													}
												}
												else
													if (!strcmp(itr->Get_word(), "DL") && is_adr == 0)
													{
														if (!is_1_ready)
														{
															opr1.reg.is_reg = 1;
															opr1.reg.r_bit = 8;
															opr1.reg.r_num = 2;
														}
														else
														{
															opr2.reg.is_reg = 1;
															opr2.reg.r_bit = 8;
															opr2.reg.r_num = 2;
														}
													}
													else
														if (!strcmp(itr->Get_word(), "BL") && is_adr == 0)
														{
															if (!is_1_ready)
															{
																opr1.reg.is_reg = 1;
																opr1.reg.r_bit = 8;
																opr1.reg.r_num = 3;
															}
															else
															{
																opr2.reg.is_reg = 1;
																opr2.reg.r_bit = 8;
																opr2.reg.r_num = 3;
															}
														}
														else
															if (!strcmp(itr->Get_word(), "AH") && is_adr == 0)
															{
																if (!is_1_ready)
																{
																	opr1.reg.is_reg = 1;
																	opr1.reg.r_bit = 8;
																	opr1.reg.r_num = 4;
																}
																else
																{
																	opr2.reg.is_reg = 1;
																	opr2.reg.r_bit = 8;
																	opr2.reg.r_num = 4;
																}
															}
															else
																if (!strcmp(itr->Get_word(), "CH") && is_adr == 0)
																{
																	if (!is_1_ready)
																	{
																		opr1.reg.is_reg = 1;
																		opr1.reg.r_bit = 8;
																		opr1.reg.r_num = 5;
																	}
																	else
																	{
																		opr2.reg.is_reg = 1;
																		opr2.reg.r_bit = 8;
																		opr2.reg.r_num = 5;
																	}
																}
																else
																	if (!strcmp(itr->Get_word(), "DH") && is_adr == 0)
																	{
																		if (!is_1_ready)
																		{
																			opr1.reg.is_reg = 1;
																			opr1.reg.r_bit = 8;
																			opr1.reg.r_num = 6;
																		}
																		else
																		{
																			opr2.reg.is_reg = 1;
																			opr2.reg.r_bit = 8;
																			opr2.reg.r_num = 6;
																		}
																	}
																	else
																		if (!strcmp(itr->Get_word(), "BH") && is_adr == 0)
																		{
																			if (!is_1_ready)
																			{
																				opr1.reg.is_reg = 1;
																				opr1.reg.r_bit = 8;
																				opr1.reg.r_num = 7;
																			}
																			else
																			{
																				opr2.reg.is_reg = 1;
																				opr2.reg.r_bit = 8;
																				opr2.reg.r_num = 7;
																			}
																		}
																		else
																			if (!strcmp(itr->Get_word(), "AX") && is_adr == 0)
																			{
																				if (!is_1_ready)
																				{
																					opr1.reg.is_reg = 1;
																					opr1.reg.r_bit = 16;
																					opr1.reg.r_num = 0;
																				}
																				else
																				{
																					opr2.reg.is_reg = 1;
																					opr2.reg.r_bit = 16;
																					opr2.reg.r_num = 0;
																				}
																			}
																			else
																				if (!strcmp(itr->Get_word(), "CX") && is_adr == 0)
																				{
																					if (!is_1_ready)
																					{
																						opr1.reg.is_reg = 1;
																						opr1.reg.r_bit = 16;
																						opr1.reg.r_num = 1;
																					}
																					else
																					{
																						opr2.reg.is_reg = 1;
																						opr2.reg.r_bit = 16;
																						opr2.reg.r_num = 1;
																					}
																				}
																				else
																					if (!strcmp(itr->Get_word(), "DX") && is_adr == 0)
																					{
																						if (!is_1_ready)
																						{
																							opr1.reg.is_reg = 1;
																							opr1.reg.r_bit = 16;
																							opr1.reg.r_num = 2;
																						}
																						else
																						{
																							opr2.reg.is_reg = 1;
																							opr2.reg.r_bit = 16;
																							opr2.reg.r_num = 2;
																						}
																					}
																					else
																						if (!strcmp(itr->Get_word(), "BX"))
																						{
																							if (is_adr == 0)
																							{
																								if (!is_1_ready)
																								{
																									opr1.reg.is_reg = 1;
																									opr1.reg.r_bit = 16;
																									opr1.reg.r_num = 3;
																								}
																								else
																								{
																									opr2.reg.is_reg = 1;
																									opr2.reg.r_bit = 16;
																									opr2.reg.r_num = 3;
																								}
																							}
																							else
																							{
																								if (!is_1_ready)
																								{
																									if (opr1.adr_reg1.is_adr_reg != 1)
																									{
																										opr1.adr_reg1.adr_bit = 16;
																										opr1.adr_reg1.adr_num = 3;
																										opr1.adr_reg1.is_adr_reg = 1;
																									}
																									else
																									{
																										opr1.adr_reg2.adr_bit = 16;
																										opr1.adr_reg2.adr_num = 3;
																										opr1.adr_reg2.is_adr_reg = 1;
																									}
																								}
																								else
																								{
																									if (opr2.adr_reg1.is_adr_reg != 1)
																									{
																										opr2.adr_reg1.adr_bit = 16;
																										opr2.adr_reg1.adr_num = 3;
																										opr2.adr_reg1.is_adr_reg = 1;
																									}
																									else
																									{
																										opr2.adr_reg2.adr_bit = 16;
																										opr2.adr_reg2.adr_num = 3;
																										opr2.adr_reg2.is_adr_reg = 1;
																									}
																								}
																							}
																						}
																						else
																							if (!strcmp(itr->Get_word(), "SP"))
																							{
																								if (!is_1_ready)
																								{
																									if (opr1.adr_reg1.is_adr_reg != 1)
																									{
																										opr1.adr_reg1.adr_bit = 16;
																										opr1.adr_reg1.adr_num = 4;
																										opr1.adr_reg1.is_adr_reg = 1;
																									}
																									else
																									{
																										opr1.adr_reg2.adr_bit = 16;
																										opr1.adr_reg2.adr_num = 4;
																										opr1.adr_reg2.is_adr_reg = 1;
																									}
																								}
																								else
																								{
																									if (opr2.adr_reg1.is_adr_reg != 1)
																									{
																										opr2.adr_reg1.adr_bit = 16;
																										opr2.adr_reg1.adr_num = 4;
																										opr2.adr_reg1.is_adr_reg = 1;
																									}
																									else
																									{
																										opr2.adr_reg2.adr_bit = 16;
																										opr2.adr_reg2.adr_num = 4;
																										opr2.adr_reg2.is_adr_reg = 1;
																									}
																								}
																							}
																							else
																								if (!strcmp(itr->Get_word(), "BP"))
																								{
																									if (!is_1_ready)
																									{
																										if (opr1.adr_reg1.is_adr_reg != 1)
																										{
																											opr1.adr_reg1.adr_bit = 16;
																											opr1.adr_reg1.adr_num = 5;
																											opr1.adr_reg1.is_adr_reg = 1;
																										}
																										else
																										{
																											opr1.adr_reg2.adr_bit = 16;
																											opr1.adr_reg2.adr_num = 5;
																											opr1.adr_reg2.is_adr_reg = 1;
																										}
																									}
																									else
																									{
																										if (opr2.adr_reg1.is_adr_reg != 1)
																										{
																											opr2.adr_reg1.adr_bit = 16;
																											opr2.adr_reg1.adr_num = 5;
																											opr2.adr_reg1.is_adr_reg = 1;
																										}
																										else
																										{
																											opr2.adr_reg2.adr_bit = 16;
																											opr2.adr_reg2.adr_num = 5;
																											opr2.adr_reg2.is_adr_reg = 1;
																										}
																									}
																								}
																								else
																									if (!strcmp(itr->Get_word(), "SI"))
																									{
																										if (!is_1_ready)
																										{
																											if (opr1.adr_reg1.is_adr_reg != 1)
																											{
																												opr1.adr_reg1.adr_bit = 16;
																												opr1.adr_reg1.adr_num = 6;
																												opr1.adr_reg1.is_adr_reg = 1;
																											}
																											else
																											{
																												opr1.adr_reg2.adr_bit = 16;
																												opr1.adr_reg2.adr_num = 6;
																												opr1.adr_reg2.is_adr_reg = 1;
																											}
																										}
																										else
																										{
																											if (opr2.adr_reg1.is_adr_reg != 1)
																											{
																												opr2.adr_reg1.adr_bit = 16;
																												opr2.adr_reg1.adr_num = 6;
																												opr2.adr_reg1.is_adr_reg = 1;
																											}
																											else
																											{
																												opr2.adr_reg2.adr_bit = 16;
																												opr2.adr_reg2.adr_num = 6;
																												opr2.adr_reg2.is_adr_reg = 1;
																											}
																										}
																									}
																									else
																										if (!strcmp(itr->Get_word(), "DI"))
																										{
																											if (!is_1_ready)
																											{
																												if (opr1.adr_reg1.is_adr_reg != 1)
																												{
																													opr1.adr_reg1.adr_bit = 16;
																													opr1.adr_reg1.adr_num = 7;
																													opr1.adr_reg1.is_adr_reg = 1;
																												}
																												else
																												{
																													opr1.adr_reg2.adr_bit = 16;
																													opr1.adr_reg2.adr_num = 7;
																													opr1.adr_reg2.is_adr_reg = 1;
																												}
																											}
																											else
																											{
																												if (opr2.adr_reg1.is_adr_reg != 1)
																												{
																													opr2.adr_reg1.adr_bit = 16;
																													opr2.adr_reg1.adr_num = 7;
																													opr2.adr_reg1.is_adr_reg = 1;
																												}
																												else
																												{
																													opr2.adr_reg2.adr_bit = 16;
																													opr2.adr_reg2.adr_num = 7;
																													opr2.adr_reg2.is_adr_reg = 1;
																												}
																											}
																										}
																										else
																											std::cout << "ERROR! Must be index or base register " <<clk<< endl;
											counts++;
										}
										else
											// "," in machine instruction
											if (!strcmp(itr->Get_word(), ",") && ind == 3 && counts > 0)
											{
												if (is_adr == 1)
													std::cout << "EXTRA SYMBOLS IN ROW " << clk << endl;
												is_1_ready = 1;
												s_item.rec.f_operand_num = num;
												s_item.rec.f_operand_len = counts;
												ind += 2;
												num += 2;
												counts = 0;
											}
											else
												// ":" like label intialization
												if (!strcmp(itr->Get_word(), ":") && s_item.rec.label_number == 1 && ind == 1)
												{
													MTable::iterator bk = itr;
													num++;
													df = 1;
												}
												else
													// other directives
													if (s_item.rec.label_number == 1 && !strcmp(itr->Get_word(), "PROC") && ind == 1) // proc
													{
														counts++;
														proc_count++;
														proc_string = itr->Get_string_number();
														s_item.rec.mem_code_num = 2;
														s_item.rec.mem_code_len = 1;
														string_offset[offs + 1] = string_offset[offs];
													}
													else
														if (ind == 1 && itr->Get_string_number() == proc_string && (!strcmp(itr->Get_word(), "NEAR") || !strcmp(itr->Get_word(), "FAR")))
														{
															counts++;
															if (!is_1_ready)
															{
																if (!strcmp(itr->Get_word(), "FAR"))
																	opr1.ptr.ptr_type = 4;
																else
																	opr1.ptr.ptr_type = 2;
															}
															else
															{
																if (!strcmp(itr->Get_word(), "FAR"))
																	opr2.ptr.ptr_type = 4;
																else
																	opr2.ptr.ptr_type = 2;
															}
														}
														else
															if (s_item.rec.label_number == 1 && !strcmp(itr->Get_word(), "ENDP") && proc_count > 0 && ind == 1)
															{
																counts++;
																proc_count--;
																string_offset[offs + 1] = string_offset[offs];
															}
															else
																if (s_item.rec.label_number == 1 && !strcmp(itr->Get_word(), "ENDS") && in_segment > 0 && ind == 1)
																{
																	counts++;
																	in_segment--;
																	curseg.len = string_offset[offs];
																	curseg.type = 0;
																	svector.push_back(curseg);
																}
																else
																	if (s_item.rec.label_number == 0 && !strcmp(itr->Get_word(), "END"))
																	{
																		is_end = 1;
																		s_item.rec.mem_code_num = 1;
																		s_item.rec.mem_code_len = 1;
																		num++;
																		ind += 2;
																		counts = 0;
																	}
																	else
																		if (is_end && type == 7 && nx == table.end())
																		{
																			s_item.rec.f_operand_num = 2;
																			s_item.rec.f_operand_len = 1;
																			counts = 0;
																		}
																		else
																			if (ind > 2 && counts == 0 && (!strcmp(itr->Get_word(), "BYTE") || !strcmp(itr->Get_word(), "WORD") || !strcmp(itr->Get_word(), "DWORD")))
																			{
																				counts++;
																				if (!is_1_ready)
																				{
																					if (!strcmp(itr->Get_word(), "BYTE"))
																						opr1.ptr.ptr_type = 1;
																					else
																						if (!strcmp(itr->Get_word(), "WORD"))
																							opr1.ptr.ptr_type = 2;
																						else
																							opr1.ptr.ptr_type = 4;
																				}
																				else
																				{
																					if (!strcmp(itr->Get_word(), "BYTE"))
																						opr2.ptr.ptr_type = 1;
																					else
																						if (!strcmp(itr->Get_word(), "WORD"))
																							opr2.ptr.ptr_type = 2;
																						else
																							opr2.ptr.ptr_type = 4;
																				}

																			}
																			else
																				if (ind > 2 && counts == 1 && !strcmp(itr->Get_word(), "PTR"))
																				{

																					counts++;
																					if (!is_1_ready)
																						opr1.ptr.is_ptr = 1;
																					else
																						opr2.ptr.is_ptr = 1;
																				}
																				else
																					if (type == 12 && ind == 3)
																					{
																						counts++;
																					}
																					else
																						if ((type == 9 || type == 10 || type == 1) && ind > 4)
																						{
																							counts++;
																							if (!is_1_ready)
																								opr1.iconst.is_const = 1;
																							else
																								opr2.iconst.is_const = 1;
																						}
																						else
																							if (ind > 4 && counts == 0 && !strcmp(itr->Get_word(), "OFFSET"))
																							{
																								counts++;
																								if (!is_1_ready)
																									opr1.is_adr_com = 1;
																								else
																									opr2.is_adr_com = 1;
																							}
																							else
																								if (type == 11 && ind > 2)
																								{
																									counts++;
																									if (!strcmp(itr->Get_word(), "["))
																										if (is_adr == 0)
																											is_adr = 1;
																										else
																											std::cout << "EXTRA SYMBOLS IN ROW " << clk << endl;
																									else
																										if (!strcmp(itr->Get_word(), "]"))
																											if (is_adr == 1)
																												is_adr = 0;
																											else
																												std::cout << "EXTRA SYMBOLS IN ROW " << clk << endl;
																								}
																								else
																									if (!strcmp(itr->Get_word(), "WORD") || !strcmp(itr->Get_word(), "DWORD") || !strcmp(itr->Get_word(), "FAR") || !strcmp(itr->Get_word(), "NEAR") && ind == 3)
																									{
																										counts++;
																										if (!is_1_ready)
																										{
																											if (!strcmp(itr->Get_word(), "WORD") || !strcmp(itr->Get_word(), "NEAR"))
																												opr1.ptr.ptr_type = 2;
																											else
																												opr1.ptr.ptr_type = 4;
																										}
																										else
																										{
																											if (!strcmp(itr->Get_word(), "WORD") || !strcmp(itr->Get_word(), "NEAR"))
																												opr2.ptr.ptr_type = 2;
																											else
																												opr2.ptr.ptr_type = 4;
																										}
																									}
																									else
																										if (type == 7 &&  ind > 2)
																										{
																											string opr_mem = itr->Get_word();
																											Label_vector::iterator ltr;
																											Var_vector::iterator vtr;
																											Proc_vector::iterator pptr;
																											counts++;
																											for (ltr = lvector.begin(); ltr != lvector.end(); ltr++)
																												if (opr_mem == ltr->name)
																												{
																													if (!is_1_ready)
																													{
																														opr1.label.is_label = 1;
																														opr1.label.l_str_num = ltr->offset;
																													}
																													else
																													{
																														opr2.label.is_label = 1;
																														opr2.label.l_str_num = ltr->offset;
																													}
																													break;
																												}
																											for (vtr = vvector.begin(); vtr != vvector.end(); vtr++)
																												if (opr_mem == vtr->name)
																												{
																													if (!is_1_ready)
																													{
																														opr1.undentf.is_undtf = 1;
																														opr1.undentf.seg = vtr->segment;
																														opr1.undentf.type = vtr->size;
																													}
																													else
																													{
																														opr2.undentf.is_undtf = 1;
																														opr2.undentf.seg = vtr->segment;
																														opr2.undentf.type = vtr->size;
																													}
																													break;
																												}
																											for (pptr = pvector.begin(); pptr != pvector.end(); pptr++)
																												if (opr_mem == pptr->name)
																												{
																													if (!is_1_ready)
																													{
																														opr1.label.is_label = 1;
																														opr1.label.l_str_num = pptr->offset;
																													}
																													else
																													{
																														opr2.label.is_label = 1;
																														opr2.label.l_str_num = pptr->offset;
																													}
																													break;
																												}

																											if (vtr == vvector.end() && ltr == lvector.end() && pptr == pvector.end())
																											{
																												string ss1;
																												MTable::iterator nitr = itr;
																												int is_next_segm = 0;
																												while (nitr != table.end())
																												{
																													ss1 = nitr->Get_word();
																													if (!strcmp(nitr->Get_word(), "ENDS"))
																														is_next_segm = 1;
																													nitr++;
																													if (opr_mem == ss1 && nitr != table.end() && (!strcmp(nitr->Get_word(), ":")
																														|| !strcmp(nitr->Get_word(), "PROC") || nitr->Get_type() == 6))
																													{
																														if (nitr->Get_type() == 6)
																														{
																															if (!is_1_ready)
																																opr1.undentf.type = nitr->Get_type();
																															else
																																opr2.undentf.type = nitr->Get_type();
																														}
																														else
																														{
																															if (is_next_segm)
																																if (!is_1_ready)
																																{
																																	opr1.undentf.type = 4;
																																	opr1.undentf.is_undtf = 1;
																																}
																																else
																																{
																																	opr2.undentf.type = 4;
																																	opr2.undentf.is_undtf = 1;
																																}
																															else
																																if (!is_1_ready)
																																{
																																	opr1.undentf.type = 2;
																																	opr1.undentf.is_undtf = 1;
																																}
																																else
																																{
																																	opr2.undentf.type = 2;
																																	opr2.undentf.is_undtf = 1;
																																}
																														}
																														break;
																													}
																													if (nitr == table.end())
																														std::cout << "ERROR, UNKNOWN ITEM IN " << clk << endl;
																												}
																											}
																										}
																										else
																										{
																											counts = -1;
																											error_row = clk;
																											std::cout << "ERROR! UNKONW FORM " << clk << endl;
																										}
					}
				}
				//next is end
				else
				{
					if (is_end && itr->Get_type() == 7)
					{
						counts++;
					}
				}
				if (((++itr) == table.end() || itr->Get_string_number() == next_string) && !df)
				{
					s_item.arr[ind] = num;
					ind++;
					s_item.arr[ind] = counts;
				}
			}
			if (is_create_opr && is_comand)
			{
				ofstream f1;
				f1.open("List2.lst", ios_base::app);
				f1 <<"00"<< get16(string_offset[offs]) << "  ";
				f1.close();
				string_offset[offs + 1] = string_offset[offs] + get_offset(opr1, opr2, comand_num, assume,ascount);
				f1.open("List2.lst", ios_base::app);
				while (itr1 != table.end() && itr1->Get_string_number() != next_string)
				{
					std::cout << itr1->Get_word() << " ";
					f1 << itr1->Get_word() << " ";
					itr1++;
				}
				f1 << endl;
				std::cout << endl;
				f1.close();
				f.open("Operands.txt", ios_base::app);
				for (itrout; itrout != table.end() && itrout->Get_string_number() < next_string; itrout++)
					f << "    "<<itrout->Get_word() << " ";
				f << endl;
				f.close();
				print_operand(opr1);
				print_operand(opr2);
			}
			offs++;
			ss_tab.push_back(s_item);
		}
		
	}
	itr = table.begin();
	std::cout << endl << endl;
	f.open("List1.lst");
	for (size_t i = 0; i < max_str - 1; i++)
	{
		std::cout << get16(string_offset[i])<<"		";
		f << get16(string_offset[i])<< "		";
		while (itr != table.end() && itr->Get_string_number() == i + 1)
		{
			std::cout << itr->Get_word() << " ";
			f << itr->Get_word() << " ";
			itr++;
		}
		std::cout << endl;
		f << endl;
	}
	f.close();*/
	return 0;
}
