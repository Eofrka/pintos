
echo:     file format elf32-i386

Disassembly of section .text:

080480a0 <main>:
 80480a0:	8d 4c 24 04          	lea    0x4(%esp),%ecx
 80480a4:	83 e4 f0             	and    $0xfffffff0,%esp
 80480a7:	ff 71 fc             	pushl  -0x4(%ecx)
 80480aa:	55                   	push   %ebp
 80480ab:	89 e5                	mov    %esp,%ebp
 80480ad:	57                   	push   %edi
 80480ae:	56                   	push   %esi
 80480af:	53                   	push   %ebx
 80480b0:	51                   	push   %ecx
 80480b1:	83 ec 08             	sub    $0x8,%esp
 80480b4:	8b 31                	mov    (%ecx),%esi
 80480b6:	8b 79 04             	mov    0x4(%ecx),%edi
 80480b9:	85 f6                	test   %esi,%esi
 80480bb:	7e 1f                	jle    80480dc <main+0x3c>
 80480bd:	bb 00 00 00 00       	mov    $0x0,%ebx
 80480c2:	8b 04 9f             	mov    (%edi,%ebx,4),%eax
 80480c5:	89 44 24 04          	mov    %eax,0x4(%esp)
 80480c9:	c7 04 24 fe 9e 04 08 	movl   $0x8049efe,(%esp)
 80480d0:	e8 47 0a 00 00       	call   8048b1c <printf>
 80480d5:	83 c3 01             	add    $0x1,%ebx
 80480d8:	39 f3                	cmp    %esi,%ebx
 80480da:	75 e6                	jne    80480c2 <main+0x22>
 80480dc:	c7 04 24 0a 00 00 00 	movl   $0xa,(%esp)
 80480e3:	e8 bf 1a 00 00       	call   8049ba7 <putchar>
 80480e8:	b8 00 00 00 00       	mov    $0x0,%eax
 80480ed:	83 c4 08             	add    $0x8,%esp
 80480f0:	59                   	pop    %ecx
 80480f1:	5b                   	pop    %ebx
 80480f2:	5e                   	pop    %esi
 80480f3:	5f                   	pop    %edi
 80480f4:	5d                   	pop    %ebp
 80480f5:	8d 61 fc             	lea    -0x4(%ecx),%esp
 80480f8:	c3                   	ret    
 80480f9:	90                   	nop    
 80480fa:	90                   	nop    
 80480fb:	90                   	nop    

080480fc <_start>:
 80480fc:	55                   	push   %ebp
 80480fd:	89 e5                	mov    %esp,%ebp
 80480ff:	83 ec 08             	sub    $0x8,%esp
 8048102:	8b 45 0c             	mov    0xc(%ebp),%eax
 8048105:	89 44 24 04          	mov    %eax,0x4(%esp)
 8048109:	8b 45 08             	mov    0x8(%ebp),%eax
 804810c:	89 04 24             	mov    %eax,(%esp)
 804810f:	e8 8c ff ff ff       	call   80480a0 <main>
 8048114:	89 04 24             	mov    %eax,(%esp)
 8048117:	e8 52 19 00 00       	call   8049a6e <exit>
 804811c:	90                   	nop    
 804811d:	90                   	nop    
 804811e:	90                   	nop    
 804811f:	90                   	nop    

08048120 <vsnprintf_helper>:
 8048120:	55                   	push   %ebp
 8048121:	89 e5                	mov    %esp,%ebp
 8048123:	83 ec 08             	sub    $0x8,%esp
 8048126:	89 1c 24             	mov    %ebx,(%esp)
 8048129:	89 74 24 04          	mov    %esi,0x4(%esp)
 804812d:	8b 75 08             	mov    0x8(%ebp),%esi
 8048130:	8b 5d 0c             	mov    0xc(%ebp),%ebx
 8048133:	8b 43 04             	mov    0x4(%ebx),%eax
 8048136:	8b 4b 08             	mov    0x8(%ebx),%ecx
 8048139:	8d 50 01             	lea    0x1(%eax),%edx
 804813c:	89 53 04             	mov    %edx,0x4(%ebx)
 804813f:	39 c8                	cmp    %ecx,%eax
 8048141:	7d 0b                	jge    804814e <vsnprintf_helper+0x2e>
 8048143:	8b 03                	mov    (%ebx),%eax
 8048145:	89 f2                	mov    %esi,%edx
 8048147:	88 10                	mov    %dl,(%eax)
 8048149:	83 c0 01             	add    $0x1,%eax
 804814c:	89 03                	mov    %eax,(%ebx)
 804814e:	8b 1c 24             	mov    (%esp),%ebx
 8048151:	8b 74 24 04          	mov    0x4(%esp),%esi
 8048155:	89 ec                	mov    %ebp,%esp
 8048157:	5d                   	pop    %ebp
 8048158:	c3                   	ret    

08048159 <output_dup>:
 8048159:	55                   	push   %ebp
 804815a:	89 e5                	mov    %esp,%ebp
 804815c:	57                   	push   %edi
 804815d:	56                   	push   %esi
 804815e:	53                   	push   %ebx
 804815f:	83 ec 0c             	sub    $0xc,%esp
 8048162:	89 d7                	mov    %edx,%edi
 8048164:	89 4d f0             	mov    %ecx,-0x10(%ebp)
 8048167:	85 d2                	test   %edx,%edx
 8048169:	74 1c                	je     8048187 <output_dup+0x2e>
 804816b:	bb 00 00 00 00       	mov    $0x0,%ebx
 8048170:	0f be f0             	movsbl %al,%esi
 8048173:	8b 45 08             	mov    0x8(%ebp),%eax
 8048176:	89 44 24 04          	mov    %eax,0x4(%esp)
 804817a:	89 34 24             	mov    %esi,(%esp)
 804817d:	ff 55 f0             	call   *-0x10(%ebp)
 8048180:	83 c3 01             	add    $0x1,%ebx
 8048183:	39 fb                	cmp    %edi,%ebx
 8048185:	75 ec                	jne    8048173 <output_dup+0x1a>
 8048187:	83 c4 0c             	add    $0xc,%esp
 804818a:	5b                   	pop    %ebx
 804818b:	5e                   	pop    %esi
 804818c:	5f                   	pop    %edi
 804818d:	5d                   	pop    %ebp
 804818e:	c3                   	ret    

0804818f <format_integer>:
 804818f:	55                   	push   %ebp
 8048190:	89 e5                	mov    %esp,%ebp
 8048192:	57                   	push   %edi
 8048193:	56                   	push   %esi
 8048194:	53                   	push   %ebx
 8048195:	83 ec 7c             	sub    $0x7c,%esp
 8048198:	89 45 98             	mov    %eax,-0x68(%ebp)
 804819b:	89 55 9c             	mov    %edx,-0x64(%ebp)
 804819e:	0f b6 55 08          	movzbl 0x8(%ebp),%edx
 80481a2:	84 c9                	test   %cl,%cl
 80481a4:	74 38                	je     80481de <format_integer+0x4f>
 80481a6:	8b 4d 10             	mov    0x10(%ebp),%ecx
 80481a9:	8b 01                	mov    (%ecx),%eax
 80481ab:	a8 02                	test   $0x2,%al
 80481ad:	74 10                	je     80481bf <format_integer+0x30>
 80481af:	80 fa 01             	cmp    $0x1,%dl
 80481b2:	19 f6                	sbb    %esi,%esi
 80481b4:	83 e6 fe             	and    $0xfffffffe,%esi
 80481b7:	83 c6 2d             	add    $0x2d,%esi
 80481ba:	89 75 ac             	mov    %esi,-0x54(%ebp)
 80481bd:	eb 26                	jmp    80481e5 <format_integer+0x56>
 80481bf:	a8 04                	test   $0x4,%al
 80481c1:	74 10                	je     80481d3 <format_integer+0x44>
 80481c3:	80 fa 01             	cmp    $0x1,%dl
 80481c6:	19 c0                	sbb    %eax,%eax
 80481c8:	83 e0 f3             	and    $0xfffffff3,%eax
 80481cb:	83 c0 2d             	add    $0x2d,%eax
 80481ce:	89 45 ac             	mov    %eax,-0x54(%ebp)
 80481d1:	eb 12                	jmp    80481e5 <format_integer+0x56>
 80481d3:	c7 45 ac 2d 00 00 00 	movl   $0x2d,-0x54(%ebp)
 80481da:	84 d2                	test   %dl,%dl
 80481dc:	75 07                	jne    80481e5 <format_integer+0x56>
 80481de:	c7 45 ac 00 00 00 00 	movl   $0x0,-0x54(%ebp)
 80481e5:	8b 55 98             	mov    -0x68(%ebp),%edx
 80481e8:	0b 55 9c             	or     -0x64(%ebp),%edx
 80481eb:	0f 95 c0             	setne  %al
 80481ee:	8b 4d 10             	mov    0x10(%ebp),%ecx
 80481f1:	f6 01 08             	testb  $0x8,(%ecx)
 80481f4:	74 04                	je     80481fa <format_integer+0x6b>
 80481f6:	84 c0                	test   %al,%al
 80481f8:	75 09                	jne    8048203 <format_integer+0x74>
 80481fa:	c7 45 a8 00 00 00 00 	movl   $0x0,-0x58(%ebp)
 8048201:	eb 09                	jmp    804820c <format_integer+0x7d>
 8048203:	8b 55 0c             	mov    0xc(%ebp),%edx
 8048206:	8b 52 08             	mov    0x8(%edx),%edx
 8048209:	89 55 a8             	mov    %edx,-0x58(%ebp)
 804820c:	84 c0                	test   %al,%al
 804820e:	75 0b                	jne    804821b <format_integer+0x8c>
 8048210:	8d 4d b0             	lea    -0x50(%ebp),%ecx
 8048213:	89 4d a4             	mov    %ecx,-0x5c(%ebp)
 8048216:	e9 a3 00 00 00       	jmp    80482be <format_integer+0x12f>
 804821b:	8d 75 b0             	lea    -0x50(%ebp),%esi
 804821e:	89 75 a4             	mov    %esi,-0x5c(%ebp)
 8048221:	bf 00 00 00 00       	mov    $0x0,%edi
 8048226:	eb 2c                	jmp    8048254 <format_integer+0xc5>
 8048228:	83 c7 01             	add    $0x1,%edi
 804822b:	8b 45 10             	mov    0x10(%ebp),%eax
 804822e:	f6 00 20             	testb  $0x20,(%eax)
 8048231:	74 21                	je     8048254 <format_integer+0xc5>
 8048233:	85 ff                	test   %edi,%edi
 8048235:	7e 1d                	jle    8048254 <format_integer+0xc5>
 8048237:	89 fa                	mov    %edi,%edx
 8048239:	8b 4d 0c             	mov    0xc(%ebp),%ecx
 804823c:	89 f8                	mov    %edi,%eax
 804823e:	c1 fa 1f             	sar    $0x1f,%edx
 8048241:	f7 79 0c             	idivl  0xc(%ecx)
 8048244:	85 d2                	test   %edx,%edx
 8048246:	75 0c                	jne    8048254 <format_integer+0xc5>
 8048248:	8b 75 a4             	mov    -0x5c(%ebp),%esi
 804824b:	c6 06 2c             	movb   $0x2c,(%esi)
 804824e:	83 c6 01             	add    $0x1,%esi
 8048251:	89 75 a4             	mov    %esi,-0x5c(%ebp)
 8048254:	8b 45 0c             	mov    0xc(%ebp),%eax
 8048257:	8b 18                	mov    (%eax),%ebx
 8048259:	89 de                	mov    %ebx,%esi
 804825b:	c1 fe 1f             	sar    $0x1f,%esi
 804825e:	89 5c 24 08          	mov    %ebx,0x8(%esp)
 8048262:	89 74 24 0c          	mov    %esi,0xc(%esp)
 8048266:	8b 55 98             	mov    -0x68(%ebp),%edx
 8048269:	8b 4d 9c             	mov    -0x64(%ebp),%ecx
 804826c:	89 14 24             	mov    %edx,(%esp)
 804826f:	89 4c 24 04          	mov    %ecx,0x4(%esp)
 8048273:	e8 fe 15 00 00       	call   8049876 <__umoddi3>
 8048278:	89 45 90             	mov    %eax,-0x70(%ebp)
 804827b:	89 55 94             	mov    %edx,-0x6c(%ebp)
 804827e:	8b 45 0c             	mov    0xc(%ebp),%eax
 8048281:	8b 48 04             	mov    0x4(%eax),%ecx
 8048284:	8b 45 90             	mov    -0x70(%ebp),%eax
 8048287:	0f b6 04 01          	movzbl (%ecx,%eax,1),%eax
 804828b:	8b 55 a4             	mov    -0x5c(%ebp),%edx
 804828e:	88 02                	mov    %al,(%edx)
 8048290:	83 c2 01             	add    $0x1,%edx
 8048293:	89 55 a4             	mov    %edx,-0x5c(%ebp)
 8048296:	89 5c 24 08          	mov    %ebx,0x8(%esp)
 804829a:	89 74 24 0c          	mov    %esi,0xc(%esp)
 804829e:	8b 45 98             	mov    -0x68(%ebp),%eax
 80482a1:	8b 55 9c             	mov    -0x64(%ebp),%edx
 80482a4:	89 04 24             	mov    %eax,(%esp)
 80482a7:	89 54 24 04          	mov    %edx,0x4(%esp)
 80482ab:	e8 a1 15 00 00       	call   8049851 <__udivdi3>
 80482b0:	89 45 98             	mov    %eax,-0x68(%ebp)
 80482b3:	89 55 9c             	mov    %edx,-0x64(%ebp)
 80482b6:	09 c2                	or     %eax,%edx
 80482b8:	0f 85 6a ff ff ff    	jne    8048228 <format_integer+0x99>
 80482be:	8b 55 10             	mov    0x10(%ebp),%edx
 80482c1:	8b 4a 08             	mov    0x8(%edx),%ecx
 80482c4:	85 c9                	test   %ecx,%ecx
 80482c6:	79 05                	jns    80482cd <format_integer+0x13e>
 80482c8:	b9 01 00 00 00       	mov    $0x1,%ecx
 80482cd:	8d 55 b0             	lea    -0x50(%ebp),%edx
 80482d0:	8b 45 a4             	mov    -0x5c(%ebp),%eax
 80482d3:	29 d0                	sub    %edx,%eax
 80482d5:	39 c1                	cmp    %eax,%ecx
 80482d7:	7e 2a                	jle    8048303 <format_integer+0x174>
 80482d9:	8d 45 ef             	lea    -0x11(%ebp),%eax
 80482dc:	3b 45 a4             	cmp    -0x5c(%ebp),%eax
 80482df:	76 22                	jbe    8048303 <format_integer+0x174>
 80482e1:	8b 45 a4             	mov    -0x5c(%ebp),%eax
 80482e4:	83 c0 01             	add    $0x1,%eax
 80482e7:	29 d0                	sub    %edx,%eax
 80482e9:	8d 5d ef             	lea    -0x11(%ebp),%ebx
 80482ec:	8b 75 a4             	mov    -0x5c(%ebp),%esi
 80482ef:	c6 06 30             	movb   $0x30,(%esi)
 80482f2:	83 c6 01             	add    $0x1,%esi
 80482f5:	89 75 a4             	mov    %esi,-0x5c(%ebp)
 80482f8:	39 c1                	cmp    %eax,%ecx
 80482fa:	7e 07                	jle    8048303 <format_integer+0x174>
 80482fc:	83 c0 01             	add    $0x1,%eax
 80482ff:	39 f3                	cmp    %esi,%ebx
 8048301:	77 e9                	ja     80482ec <format_integer+0x15d>
 8048303:	8b 45 10             	mov    0x10(%ebp),%eax
 8048306:	f6 00 08             	testb  $0x8,(%eax)
 8048309:	74 25                	je     8048330 <format_integer+0x1a1>
 804830b:	8b 4d 0c             	mov    0xc(%ebp),%ecx
 804830e:	83 39 08             	cmpl   $0x8,(%ecx)
 8048311:	75 1d                	jne    8048330 <format_integer+0x1a1>
 8048313:	8d 45 b0             	lea    -0x50(%ebp),%eax
 8048316:	3b 45 a4             	cmp    -0x5c(%ebp),%eax
 8048319:	74 09                	je     8048324 <format_integer+0x195>
 804831b:	8b 75 a4             	mov    -0x5c(%ebp),%esi
 804831e:	80 7e ff 30          	cmpb   $0x30,-0x1(%esi)
 8048322:	74 0c                	je     8048330 <format_integer+0x1a1>
 8048324:	8b 45 a4             	mov    -0x5c(%ebp),%eax
 8048327:	c6 00 30             	movb   $0x30,(%eax)
 804832a:	83 c0 01             	add    $0x1,%eax
 804832d:	89 45 a4             	mov    %eax,-0x5c(%ebp)
 8048330:	83 7d a8 01          	cmpl   $0x1,-0x58(%ebp)
 8048334:	19 c0                	sbb    %eax,%eax
 8048336:	f7 d0                	not    %eax
 8048338:	83 e0 02             	and    $0x2,%eax
 804833b:	2b 55 a4             	sub    -0x5c(%ebp),%edx
 804833e:	8b 4d 10             	mov    0x10(%ebp),%ecx
 8048341:	03 51 04             	add    0x4(%ecx),%edx
 8048344:	29 c2                	sub    %eax,%edx
 8048346:	83 7d ac 00          	cmpl   $0x0,-0x54(%ebp)
 804834a:	0f 95 c0             	setne  %al
 804834d:	0f b6 c0             	movzbl %al,%eax
 8048350:	29 c2                	sub    %eax,%edx
 8048352:	89 d0                	mov    %edx,%eax
 8048354:	c1 f8 1f             	sar    $0x1f,%eax
 8048357:	f7 d0                	not    %eax
 8048359:	89 d6                	mov    %edx,%esi
 804835b:	21 c6                	and    %eax,%esi
 804835d:	f6 01 11             	testb  $0x11,(%ecx)
 8048360:	75 15                	jne    8048377 <format_integer+0x1e8>
 8048362:	8b 45 18             	mov    0x18(%ebp),%eax
 8048365:	89 04 24             	mov    %eax,(%esp)
 8048368:	8b 4d 14             	mov    0x14(%ebp),%ecx
 804836b:	89 f2                	mov    %esi,%edx
 804836d:	b8 20 00 00 00       	mov    $0x20,%eax
 8048372:	e8 e2 fd ff ff       	call   8048159 <output_dup>
 8048377:	83 7d ac 00          	cmpl   $0x0,-0x54(%ebp)
 804837b:	74 10                	je     804838d <format_integer+0x1fe>
 804837d:	8b 55 18             	mov    0x18(%ebp),%edx
 8048380:	89 54 24 04          	mov    %edx,0x4(%esp)
 8048384:	8b 4d ac             	mov    -0x54(%ebp),%ecx
 8048387:	89 0c 24             	mov    %ecx,(%esp)
 804838a:	ff 55 14             	call   *0x14(%ebp)
 804838d:	83 7d a8 00          	cmpl   $0x0,-0x58(%ebp)
 8048391:	74 22                	je     80483b5 <format_integer+0x226>
 8048393:	8b 45 18             	mov    0x18(%ebp),%eax
 8048396:	89 44 24 04          	mov    %eax,0x4(%esp)
 804839a:	c7 04 24 30 00 00 00 	movl   $0x30,(%esp)
 80483a1:	ff 55 14             	call   *0x14(%ebp)
 80483a4:	8b 55 18             	mov    0x18(%ebp),%edx
 80483a7:	89 54 24 04          	mov    %edx,0x4(%esp)
 80483ab:	0f be 45 a8          	movsbl -0x58(%ebp),%eax
 80483af:	89 04 24             	mov    %eax,(%esp)
 80483b2:	ff 55 14             	call   *0x14(%ebp)
 80483b5:	8b 4d 10             	mov    0x10(%ebp),%ecx
 80483b8:	f6 01 10             	testb  $0x10,(%ecx)
 80483bb:	74 15                	je     80483d2 <format_integer+0x243>
 80483bd:	8b 45 18             	mov    0x18(%ebp),%eax
 80483c0:	89 04 24             	mov    %eax,(%esp)
 80483c3:	8b 4d 14             	mov    0x14(%ebp),%ecx
 80483c6:	89 f2                	mov    %esi,%edx
 80483c8:	b8 30 00 00 00       	mov    $0x30,%eax
 80483cd:	e8 87 fd ff ff       	call   8048159 <output_dup>
 80483d2:	8d 45 b0             	lea    -0x50(%ebp),%eax
 80483d5:	3b 45 a4             	cmp    -0x5c(%ebp),%eax
 80483d8:	73 1d                	jae    80483f7 <format_integer+0x268>
 80483da:	8b 5d a4             	mov    -0x5c(%ebp),%ebx
 80483dd:	89 c7                	mov    %eax,%edi
 80483df:	8b 55 18             	mov    0x18(%ebp),%edx
 80483e2:	89 54 24 04          	mov    %edx,0x4(%esp)
 80483e6:	0f be 43 ff          	movsbl -0x1(%ebx),%eax
 80483ea:	89 04 24             	mov    %eax,(%esp)
 80483ed:	ff 55 14             	call   *0x14(%ebp)
 80483f0:	83 eb 01             	sub    $0x1,%ebx
 80483f3:	39 df                	cmp    %ebx,%edi
 80483f5:	72 e8                	jb     80483df <format_integer+0x250>
 80483f7:	8b 4d 10             	mov    0x10(%ebp),%ecx
 80483fa:	f6 01 01             	testb  $0x1,(%ecx)
 80483fd:	74 15                	je     8048414 <format_integer+0x285>
 80483ff:	8b 45 18             	mov    0x18(%ebp),%eax
 8048402:	89 04 24             	mov    %eax,(%esp)
 8048405:	8b 4d 14             	mov    0x14(%ebp),%ecx
 8048408:	89 f2                	mov    %esi,%edx
 804840a:	b8 20 00 00 00       	mov    $0x20,%eax
 804840f:	e8 45 fd ff ff       	call   8048159 <output_dup>
 8048414:	83 c4 7c             	add    $0x7c,%esp
 8048417:	5b                   	pop    %ebx
 8048418:	5e                   	pop    %esi
 8048419:	5f                   	pop    %edi
 804841a:	5d                   	pop    %ebp
 804841b:	c3                   	ret    

0804841c <format_string>:
 804841c:	55                   	push   %ebp
 804841d:	89 e5                	mov    %esp,%ebp
 804841f:	57                   	push   %edi
 8048420:	56                   	push   %esi
 8048421:	53                   	push   %ebx
 8048422:	83 ec 0c             	sub    $0xc,%esp
 8048425:	89 c7                	mov    %eax,%edi
 8048427:	89 d6                	mov    %edx,%esi
 8048429:	89 4d f0             	mov    %ecx,-0x10(%ebp)
 804842c:	8b 41 04             	mov    0x4(%ecx),%eax
 804842f:	39 d0                	cmp    %edx,%eax
 8048431:	7e 1c                	jle    804844f <format_string+0x33>
 8048433:	f6 01 01             	testb  $0x1,(%ecx)
 8048436:	75 17                	jne    804844f <format_string+0x33>
 8048438:	89 c2                	mov    %eax,%edx
 804843a:	29 f2                	sub    %esi,%edx
 804843c:	8b 45 0c             	mov    0xc(%ebp),%eax
 804843f:	89 04 24             	mov    %eax,(%esp)
 8048442:	8b 4d 08             	mov    0x8(%ebp),%ecx
 8048445:	b8 20 00 00 00       	mov    $0x20,%eax
 804844a:	e8 0a fd ff ff       	call   8048159 <output_dup>
 804844f:	85 f6                	test   %esi,%esi
 8048451:	7e 1d                	jle    8048470 <format_string+0x54>
 8048453:	bb 00 00 00 00       	mov    $0x0,%ebx
 8048458:	8b 55 0c             	mov    0xc(%ebp),%edx
 804845b:	89 54 24 04          	mov    %edx,0x4(%esp)
 804845f:	0f be 04 3b          	movsbl (%ebx,%edi,1),%eax
 8048463:	89 04 24             	mov    %eax,(%esp)
 8048466:	ff 55 08             	call   *0x8(%ebp)
 8048469:	83 c3 01             	add    $0x1,%ebx
 804846c:	39 f3                	cmp    %esi,%ebx
 804846e:	75 e8                	jne    8048458 <format_string+0x3c>
 8048470:	8b 55 f0             	mov    -0x10(%ebp),%edx
 8048473:	8b 42 04             	mov    0x4(%edx),%eax
 8048476:	39 c6                	cmp    %eax,%esi
 8048478:	7d 1c                	jge    8048496 <format_string+0x7a>
 804847a:	f6 02 01             	testb  $0x1,(%edx)
 804847d:	74 17                	je     8048496 <format_string+0x7a>
 804847f:	89 c2                	mov    %eax,%edx
 8048481:	29 f2                	sub    %esi,%edx
 8048483:	8b 45 0c             	mov    0xc(%ebp),%eax
 8048486:	89 04 24             	mov    %eax,(%esp)
 8048489:	8b 4d 08             	mov    0x8(%ebp),%ecx
 804848c:	b8 20 00 00 00       	mov    $0x20,%eax
 8048491:	e8 c3 fc ff ff       	call   8048159 <output_dup>
 8048496:	83 c4 0c             	add    $0xc,%esp
 8048499:	5b                   	pop    %ebx
 804849a:	5e                   	pop    %esi
 804849b:	5f                   	pop    %edi
 804849c:	5d                   	pop    %ebp
 804849d:	c3                   	ret    

0804849e <__vprintf>:
 804849e:	55                   	push   %ebp
 804849f:	89 e5                	mov    %esp,%ebp
 80484a1:	57                   	push   %edi
 80484a2:	56                   	push   %esi
 80484a3:	53                   	push   %ebx
 80484a4:	83 ec 5c             	sub    $0x5c,%esp
 80484a7:	8b 75 08             	mov    0x8(%ebp),%esi
 80484aa:	8b 7d 0c             	mov    0xc(%ebp),%edi
 80484ad:	0f b6 06             	movzbl (%esi),%eax
 80484b0:	84 c0                	test   %al,%al
 80484b2:	0f 84 8f 05 00 00    	je     8048a47 <__vprintf+0x5a9>
 80484b8:	3c 25                	cmp    $0x25,%al
 80484ba:	74 15                	je     80484d1 <__vprintf+0x33>
 80484bc:	8b 55 14             	mov    0x14(%ebp),%edx
 80484bf:	89 54 24 04          	mov    %edx,0x4(%esp)
 80484c3:	0f be c0             	movsbl %al,%eax
 80484c6:	89 04 24             	mov    %eax,(%esp)
 80484c9:	ff 55 10             	call   *0x10(%ebp)
 80484cc:	e9 5e 05 00 00       	jmp    8048a2f <__vprintf+0x591>
 80484d1:	83 c6 01             	add    $0x1,%esi
 80484d4:	80 3e 25             	cmpb   $0x25,(%esi)
 80484d7:	75 16                	jne    80484ef <__vprintf+0x51>
 80484d9:	8b 4d 14             	mov    0x14(%ebp),%ecx
 80484dc:	89 4c 24 04          	mov    %ecx,0x4(%esp)
 80484e0:	c7 04 24 25 00 00 00 	movl   $0x25,(%esp)
 80484e7:	ff 55 10             	call   *0x10(%ebp)
 80484ea:	e9 40 05 00 00       	jmp    8048a2f <__vprintf+0x591>
 80484ef:	c7 45 dc 00 00 00 00 	movl   $0x0,-0x24(%ebp)
 80484f6:	0f b6 06             	movzbl (%esi),%eax
 80484f9:	83 c6 01             	add    $0x1,%esi
 80484fc:	83 e8 20             	sub    $0x20,%eax
 80484ff:	3c 10                	cmp    $0x10,%al
 8048501:	77 35                	ja     8048538 <__vprintf+0x9a>
 8048503:	0f b6 c0             	movzbl %al,%eax
 8048506:	ff 24 85 b0 9c 04 08 	jmp    *0x8049cb0(,%eax,4)
 804850d:	83 4d dc 01          	orl    $0x1,-0x24(%ebp)
 8048511:	eb e3                	jmp    80484f6 <__vprintf+0x58>
 8048513:	83 4d dc 02          	orl    $0x2,-0x24(%ebp)
 8048517:	eb dd                	jmp    80484f6 <__vprintf+0x58>
 8048519:	83 4d dc 04          	orl    $0x4,-0x24(%ebp)
 804851d:	8d 76 00             	lea    0x0(%esi),%esi
 8048520:	eb d4                	jmp    80484f6 <__vprintf+0x58>
 8048522:	83 4d dc 08          	orl    $0x8,-0x24(%ebp)
 8048526:	eb ce                	jmp    80484f6 <__vprintf+0x58>
 8048528:	83 4d dc 10          	orl    $0x10,-0x24(%ebp)
 804852c:	8d 74 26 00          	lea    0x0(%esi),%esi
 8048530:	eb c4                	jmp    80484f6 <__vprintf+0x58>
 8048532:	83 4d dc 20          	orl    $0x20,-0x24(%ebp)
 8048536:	eb be                	jmp    80484f6 <__vprintf+0x58>
 8048538:	8d 4e ff             	lea    -0x1(%esi),%ecx
 804853b:	8b 45 dc             	mov    -0x24(%ebp),%eax
 804853e:	a8 01                	test   $0x1,%al
 8048540:	74 06                	je     8048548 <__vprintf+0xaa>
 8048542:	83 e0 ef             	and    $0xffffffef,%eax
 8048545:	89 45 dc             	mov    %eax,-0x24(%ebp)
 8048548:	8b 45 dc             	mov    -0x24(%ebp),%eax
 804854b:	a8 02                	test   $0x2,%al
 804854d:	74 06                	je     8048555 <__vprintf+0xb7>
 804854f:	83 e0 fb             	and    $0xfffffffb,%eax
 8048552:	89 45 dc             	mov    %eax,-0x24(%ebp)
 8048555:	c7 45 e0 00 00 00 00 	movl   $0x0,-0x20(%ebp)
 804855c:	0f b6 01             	movzbl (%ecx),%eax
 804855f:	3c 2a                	cmp    $0x2a,%al
 8048561:	74 0f                	je     8048572 <__vprintf+0xd4>
 8048563:	0f be d0             	movsbl %al,%edx
 8048566:	8d 42 d0             	lea    -0x30(%edx),%eax
 8048569:	89 f3                	mov    %esi,%ebx
 804856b:	83 f8 09             	cmp    $0x9,%eax
 804856e:	76 0f                	jbe    804857f <__vprintf+0xe1>
 8048570:	eb 3a                	jmp    80485ac <__vprintf+0x10e>
 8048572:	83 c1 01             	add    $0x1,%ecx
 8048575:	8b 07                	mov    (%edi),%eax
 8048577:	89 45 e0             	mov    %eax,-0x20(%ebp)
 804857a:	8d 7f 04             	lea    0x4(%edi),%edi
 804857d:	eb 1d                	jmp    804859c <__vprintf+0xfe>
 804857f:	8b 45 e0             	mov    -0x20(%ebp),%eax
 8048582:	8d 04 80             	lea    (%eax,%eax,4),%eax
 8048585:	8d 44 42 d0          	lea    -0x30(%edx,%eax,2),%eax
 8048589:	89 45 e0             	mov    %eax,-0x20(%ebp)
 804858c:	89 d9                	mov    %ebx,%ecx
 804858e:	0f be 13             	movsbl (%ebx),%edx
 8048591:	8d 5b 01             	lea    0x1(%ebx),%ebx
 8048594:	8d 42 d0             	lea    -0x30(%edx),%eax
 8048597:	83 f8 09             	cmp    $0x9,%eax
 804859a:	76 e3                	jbe    804857f <__vprintf+0xe1>
 804859c:	8b 45 e0             	mov    -0x20(%ebp),%eax
 804859f:	85 c0                	test   %eax,%eax
 80485a1:	79 09                	jns    80485ac <__vprintf+0x10e>
 80485a3:	f7 d8                	neg    %eax
 80485a5:	89 45 e0             	mov    %eax,-0x20(%ebp)
 80485a8:	83 4d dc 01          	orl    $0x1,-0x24(%ebp)
 80485ac:	c7 45 e4 ff ff ff ff 	movl   $0xffffffff,-0x1c(%ebp)
 80485b3:	80 39 2e             	cmpb   $0x2e,(%ecx)
 80485b6:	75 57                	jne    804860f <__vprintf+0x171>
 80485b8:	83 c1 01             	add    $0x1,%ecx
 80485bb:	80 39 2a             	cmpb   $0x2a,(%ecx)
 80485be:	75 0d                	jne    80485cd <__vprintf+0x12f>
 80485c0:	83 c1 01             	add    $0x1,%ecx
 80485c3:	8b 07                	mov    (%edi),%eax
 80485c5:	89 45 e4             	mov    %eax,-0x1c(%ebp)
 80485c8:	8d 7f 04             	lea    0x4(%edi),%edi
 80485cb:	eb 31                	jmp    80485fe <__vprintf+0x160>
 80485cd:	c7 45 e4 00 00 00 00 	movl   $0x0,-0x1c(%ebp)
 80485d4:	0f be 11             	movsbl (%ecx),%edx
 80485d7:	8d 42 d0             	lea    -0x30(%edx),%eax
 80485da:	83 f8 09             	cmp    $0x9,%eax
 80485dd:	0f 87 5b 04 00 00    	ja     8048a3e <__vprintf+0x5a0>
 80485e3:	8b 45 e4             	mov    -0x1c(%ebp),%eax
 80485e6:	8d 04 80             	lea    (%eax,%eax,4),%eax
 80485e9:	8d 44 42 d0          	lea    -0x30(%edx,%eax,2),%eax
 80485ed:	89 45 e4             	mov    %eax,-0x1c(%ebp)
 80485f0:	83 c1 01             	add    $0x1,%ecx
 80485f3:	0f be 11             	movsbl (%ecx),%edx
 80485f6:	8d 42 d0             	lea    -0x30(%edx),%eax
 80485f9:	83 f8 09             	cmp    $0x9,%eax
 80485fc:	76 e5                	jbe    80485e3 <__vprintf+0x145>
 80485fe:	83 7d e4 00          	cmpl   $0x0,-0x1c(%ebp)
 8048602:	0f 89 36 04 00 00    	jns    8048a3e <__vprintf+0x5a0>
 8048608:	c7 45 e4 ff ff ff ff 	movl   $0xffffffff,-0x1c(%ebp)
 804860f:	c7 45 e8 03 00 00 00 	movl   $0x3,-0x18(%ebp)
 8048616:	8d 71 01             	lea    0x1(%ecx),%esi
 8048619:	0f b6 01             	movzbl (%ecx),%eax
 804861c:	83 e8 68             	sub    $0x68,%eax
 804861f:	3c 12                	cmp    $0x12,%al
 8048621:	77 5f                	ja     8048682 <__vprintf+0x1e4>
 8048623:	0f b6 c0             	movzbl %al,%eax
 8048626:	ff 24 85 f4 9c 04 08 	jmp    *0x8049cf4(,%eax,4)
 804862d:	80 3e 68             	cmpb   $0x68,(%esi)
 8048630:	75 0c                	jne    804863e <__vprintf+0x1a0>
 8048632:	83 c6 01             	add    $0x1,%esi
 8048635:	c7 45 e8 01 00 00 00 	movl   $0x1,-0x18(%ebp)
 804863c:	eb 47                	jmp    8048685 <__vprintf+0x1e7>
 804863e:	c7 45 e8 02 00 00 00 	movl   $0x2,-0x18(%ebp)
 8048645:	eb 3e                	jmp    8048685 <__vprintf+0x1e7>
 8048647:	c7 45 e8 04 00 00 00 	movl   $0x4,-0x18(%ebp)
 804864e:	66 90                	xchg   %ax,%ax
 8048650:	eb 33                	jmp    8048685 <__vprintf+0x1e7>
 8048652:	80 3e 6c             	cmpb   $0x6c,(%esi)
 8048655:	75 0c                	jne    8048663 <__vprintf+0x1c5>
 8048657:	83 c6 01             	add    $0x1,%esi
 804865a:	c7 45 e8 06 00 00 00 	movl   $0x6,-0x18(%ebp)
 8048661:	eb 22                	jmp    8048685 <__vprintf+0x1e7>
 8048663:	c7 45 e8 05 00 00 00 	movl   $0x5,-0x18(%ebp)
 804866a:	eb 19                	jmp    8048685 <__vprintf+0x1e7>
 804866c:	c7 45 e8 07 00 00 00 	movl   $0x7,-0x18(%ebp)
 8048673:	eb 10                	jmp    8048685 <__vprintf+0x1e7>
 8048675:	c7 45 e8 08 00 00 00 	movl   $0x8,-0x18(%ebp)
 804867c:	8d 74 26 00          	lea    0x0(%esi),%esi
 8048680:	eb 03                	jmp    8048685 <__vprintf+0x1e7>
 8048682:	83 ee 01             	sub    $0x1,%esi
 8048685:	0f b6 0e             	movzbl (%esi),%ecx
 8048688:	0f be d1             	movsbl %cl,%edx
 804868b:	8d 41 bb             	lea    -0x45(%ecx),%eax
 804868e:	3c 33                	cmp    $0x33,%al
 8048690:	0f 87 7b 03 00 00    	ja     8048a11 <__vprintf+0x573>
 8048696:	0f b6 c0             	movzbl %al,%eax
 8048699:	ff 24 85 40 9d 04 08 	jmp    *0x8049d40(,%eax,4)
 80486a0:	83 7d e8 08          	cmpl   $0x8,-0x18(%ebp)
 80486a4:	0f 87 cf 00 00 00    	ja     8048779 <__vprintf+0x2db>
 80486aa:	8b 45 e8             	mov    -0x18(%ebp),%eax
 80486ad:	8d 76 00             	lea    0x0(%esi),%esi
 80486b0:	ff 24 85 10 9e 04 08 	jmp    *0x8049e10(,%eax,4)
 80486b7:	8d 47 04             	lea    0x4(%edi),%eax
 80486ba:	89 45 cc             	mov    %eax,-0x34(%ebp)
 80486bd:	0f be 3f             	movsbl (%edi),%edi
 80486c0:	89 fa                	mov    %edi,%edx
 80486c2:	c1 fa 1f             	sar    $0x1f,%edx
 80486c5:	89 7d b8             	mov    %edi,-0x48(%ebp)
 80486c8:	89 55 bc             	mov    %edx,-0x44(%ebp)
 80486cb:	e9 cd 00 00 00       	jmp    804879d <__vprintf+0x2ff>
 80486d0:	8d 57 04             	lea    0x4(%edi),%edx
 80486d3:	89 55 cc             	mov    %edx,-0x34(%ebp)
 80486d6:	0f bf 3f             	movswl (%edi),%edi
 80486d9:	89 fa                	mov    %edi,%edx
 80486db:	c1 fa 1f             	sar    $0x1f,%edx
 80486de:	89 7d b8             	mov    %edi,-0x48(%ebp)
 80486e1:	89 55 bc             	mov    %edx,-0x44(%ebp)
 80486e4:	e9 b4 00 00 00       	jmp    804879d <__vprintf+0x2ff>
 80486e9:	8d 57 04             	lea    0x4(%edi),%edx
 80486ec:	89 55 cc             	mov    %edx,-0x34(%ebp)
 80486ef:	8b 07                	mov    (%edi),%eax
 80486f1:	89 45 b8             	mov    %eax,-0x48(%ebp)
 80486f4:	89 c1                	mov    %eax,%ecx
 80486f6:	c1 f9 1f             	sar    $0x1f,%ecx
 80486f9:	89 4d bc             	mov    %ecx,-0x44(%ebp)
 80486fc:	e9 9c 00 00 00       	jmp    804879d <__vprintf+0x2ff>
 8048701:	8d 47 08             	lea    0x8(%edi),%eax
 8048704:	89 45 cc             	mov    %eax,-0x34(%ebp)
 8048707:	8b 17                	mov    (%edi),%edx
 8048709:	8b 4f 04             	mov    0x4(%edi),%ecx
 804870c:	89 55 b8             	mov    %edx,-0x48(%ebp)
 804870f:	89 4d bc             	mov    %ecx,-0x44(%ebp)
 8048712:	e9 86 00 00 00       	jmp    804879d <__vprintf+0x2ff>
 8048717:	8d 4f 04             	lea    0x4(%edi),%ecx
 804871a:	89 4d cc             	mov    %ecx,-0x34(%ebp)
 804871d:	8b 07                	mov    (%edi),%eax
 804871f:	89 45 b8             	mov    %eax,-0x48(%ebp)
 8048722:	89 c2                	mov    %eax,%edx
 8048724:	c1 fa 1f             	sar    $0x1f,%edx
 8048727:	89 55 bc             	mov    %edx,-0x44(%ebp)
 804872a:	eb 71                	jmp    804879d <__vprintf+0x2ff>
 804872c:	8d 4f 08             	lea    0x8(%edi),%ecx
 804872f:	89 4d cc             	mov    %ecx,-0x34(%ebp)
 8048732:	8b 07                	mov    (%edi),%eax
 8048734:	8b 57 04             	mov    0x4(%edi),%edx
 8048737:	89 45 b8             	mov    %eax,-0x48(%ebp)
 804873a:	89 55 bc             	mov    %edx,-0x44(%ebp)
 804873d:	eb 5e                	jmp    804879d <__vprintf+0x2ff>
 804873f:	8d 57 04             	lea    0x4(%edi),%edx
 8048742:	89 55 cc             	mov    %edx,-0x34(%ebp)
 8048745:	8b 07                	mov    (%edi),%eax
 8048747:	89 45 b8             	mov    %eax,-0x48(%ebp)
 804874a:	89 c1                	mov    %eax,%ecx
 804874c:	c1 f9 1f             	sar    $0x1f,%ecx
 804874f:	89 4d bc             	mov    %ecx,-0x44(%ebp)
 8048752:	eb 49                	jmp    804879d <__vprintf+0x2ff>
 8048754:	8d 47 04             	lea    0x4(%edi),%eax
 8048757:	89 45 cc             	mov    %eax,-0x34(%ebp)
 804875a:	8b 17                	mov    (%edi),%edx
 804875c:	b9 00 00 00 00       	mov    $0x0,%ecx
 8048761:	89 55 b8             	mov    %edx,-0x48(%ebp)
 8048764:	89 4d bc             	mov    %ecx,-0x44(%ebp)
 8048767:	81 fa ff ff ff 7f    	cmp    $0x7fffffff,%edx
 804876d:	76 2e                	jbe    804879d <__vprintf+0x2ff>
 804876f:	83 45 b8 00          	addl   $0x0,-0x48(%ebp)
 8048773:	83 55 bc ff          	adcl   $0xffffffff,-0x44(%ebp)
 8048777:	eb 24                	jmp    804879d <__vprintf+0x2ff>
 8048779:	c7 44 24 0c b8 a0 04 	movl   $0x804a0b8,0xc(%esp)
 8048780:	08 
 8048781:	c7 44 24 08 98 9e 04 	movl   $0x8049e98,0x8(%esp)
 8048788:	08 
 8048789:	c7 44 24 04 dc 00 00 	movl   $0xdc,0x4(%esp)
 8048790:	00 
 8048791:	c7 04 24 02 9f 04 08 	movl   $0x8049f02,(%esp)
 8048798:	e8 1f 11 00 00       	call   80498bc <debug_panic>
 804879d:	8b 5d bc             	mov    -0x44(%ebp),%ebx
 80487a0:	c1 fb 1f             	sar    $0x1f,%ebx
 80487a3:	8b 45 b8             	mov    -0x48(%ebp),%eax
 80487a6:	31 d8                	xor    %ebx,%eax
 80487a8:	8b 55 bc             	mov    -0x44(%ebp),%edx
 80487ab:	31 da                	xor    %ebx,%edx
 80487ad:	29 d8                	sub    %ebx,%eax
 80487af:	19 da                	sbb    %ebx,%edx
 80487b1:	8b 4d 14             	mov    0x14(%ebp),%ecx
 80487b4:	89 4c 24 10          	mov    %ecx,0x10(%esp)
 80487b8:	8b 4d 10             	mov    0x10(%ebp),%ecx
 80487bb:	89 4c 24 0c          	mov    %ecx,0xc(%esp)
 80487bf:	8d 4d dc             	lea    -0x24(%ebp),%ecx
 80487c2:	89 4c 24 08          	mov    %ecx,0x8(%esp)
 80487c6:	c7 44 24 04 58 9e 04 	movl   $0x8049e58,0x4(%esp)
 80487cd:	08 
 80487ce:	8b 4d bc             	mov    -0x44(%ebp),%ecx
 80487d1:	c1 e9 1f             	shr    $0x1f,%ecx
 80487d4:	89 0c 24             	mov    %ecx,(%esp)
 80487d7:	b9 01 00 00 00       	mov    $0x1,%ecx
 80487dc:	e8 ae f9 ff ff       	call   804818f <format_integer>
 80487e1:	8b 7d cc             	mov    -0x34(%ebp),%edi
 80487e4:	e9 46 02 00 00       	jmp    8048a2f <__vprintf+0x591>
 80487e9:	83 7d e8 08          	cmpl   $0x8,-0x18(%ebp)
 80487ed:	0f 87 9b 00 00 00    	ja     804888e <__vprintf+0x3f0>
 80487f3:	8b 45 e8             	mov    -0x18(%ebp),%eax
 80487f6:	ff 24 85 34 9e 04 08 	jmp    *0x8049e34(,%eax,4)
 80487fd:	0f b6 07             	movzbl (%edi),%eax
 8048800:	89 45 c0             	mov    %eax,-0x40(%ebp)
 8048803:	c7 45 c4 00 00 00 00 	movl   $0x0,-0x3c(%ebp)
 804880a:	8d 7f 04             	lea    0x4(%edi),%edi
 804880d:	e9 a0 00 00 00       	jmp    80488b2 <__vprintf+0x414>
 8048812:	0f b7 07             	movzwl (%edi),%eax
 8048815:	89 45 c0             	mov    %eax,-0x40(%ebp)
 8048818:	c7 45 c4 00 00 00 00 	movl   $0x0,-0x3c(%ebp)
 804881f:	8d 7f 04             	lea    0x4(%edi),%edi
 8048822:	e9 8b 00 00 00       	jmp    80488b2 <__vprintf+0x414>
 8048827:	8b 07                	mov    (%edi),%eax
 8048829:	ba 00 00 00 00       	mov    $0x0,%edx
 804882e:	89 45 c0             	mov    %eax,-0x40(%ebp)
 8048831:	89 55 c4             	mov    %edx,-0x3c(%ebp)
 8048834:	8d 7f 04             	lea    0x4(%edi),%edi
 8048837:	eb 79                	jmp    80488b2 <__vprintf+0x414>
 8048839:	8b 07                	mov    (%edi),%eax
 804883b:	8b 57 04             	mov    0x4(%edi),%edx
 804883e:	89 45 c0             	mov    %eax,-0x40(%ebp)
 8048841:	89 55 c4             	mov    %edx,-0x3c(%ebp)
 8048844:	8d 7f 08             	lea    0x8(%edi),%edi
 8048847:	eb 69                	jmp    80488b2 <__vprintf+0x414>
 8048849:	8b 07                	mov    (%edi),%eax
 804884b:	ba 00 00 00 00       	mov    $0x0,%edx
 8048850:	89 45 c0             	mov    %eax,-0x40(%ebp)
 8048853:	89 55 c4             	mov    %edx,-0x3c(%ebp)
 8048856:	8d 7f 04             	lea    0x4(%edi),%edi
 8048859:	eb 57                	jmp    80488b2 <__vprintf+0x414>
 804885b:	8b 07                	mov    (%edi),%eax
 804885d:	8b 57 04             	mov    0x4(%edi),%edx
 8048860:	89 45 c0             	mov    %eax,-0x40(%ebp)
 8048863:	89 55 c4             	mov    %edx,-0x3c(%ebp)
 8048866:	8d 7f 08             	lea    0x8(%edi),%edi
 8048869:	eb 47                	jmp    80488b2 <__vprintf+0x414>
 804886b:	8b 07                	mov    (%edi),%eax
 804886d:	89 45 c0             	mov    %eax,-0x40(%ebp)
 8048870:	c7 45 c4 00 00 00 00 	movl   $0x0,-0x3c(%ebp)
 8048877:	8d 7f 04             	lea    0x4(%edi),%edi
 804887a:	eb 36                	jmp    80488b2 <__vprintf+0x414>
 804887c:	8b 07                	mov    (%edi),%eax
 804887e:	ba 00 00 00 00       	mov    $0x0,%edx
 8048883:	89 45 c0             	mov    %eax,-0x40(%ebp)
 8048886:	89 55 c4             	mov    %edx,-0x3c(%ebp)
 8048889:	8d 7f 04             	lea    0x4(%edi),%edi
 804888c:	eb 24                	jmp    80488b2 <__vprintf+0x414>
 804888e:	c7 44 24 0c b8 a0 04 	movl   $0x804a0b8,0xc(%esp)
 8048895:	08 
 8048896:	c7 44 24 08 98 9e 04 	movl   $0x8049e98,0x8(%esp)
 804889d:	08 
 804889e:	c7 44 24 04 0b 01 00 	movl   $0x10b,0x4(%esp)
 80488a5:	00 
 80488a6:	c7 04 24 02 9f 04 08 	movl   $0x8049f02,(%esp)
 80488ad:	e8 0a 10 00 00       	call   80498bc <debug_panic>
 80488b2:	80 f9 6f             	cmp    $0x6f,%cl
 80488b5:	74 1d                	je     80488d4 <__vprintf+0x436>
 80488b7:	80 f9 6f             	cmp    $0x6f,%cl
 80488ba:	7f 08                	jg     80488c4 <__vprintf+0x426>
 80488bc:	80 f9 58             	cmp    $0x58,%cl
 80488bf:	90                   	nop    
 80488c0:	75 27                	jne    80488e9 <__vprintf+0x44b>
 80488c2:	eb 49                	jmp    804890d <__vprintf+0x46f>
 80488c4:	80 f9 75             	cmp    $0x75,%cl
 80488c7:	74 12                	je     80488db <__vprintf+0x43d>
 80488c9:	80 f9 78             	cmp    $0x78,%cl
 80488cc:	8d 74 26 00          	lea    0x0(%esi),%esi
 80488d0:	75 17                	jne    80488e9 <__vprintf+0x44b>
 80488d2:	eb 0e                	jmp    80488e2 <__vprintf+0x444>
 80488d4:	b8 68 9e 04 08       	mov    $0x8049e68,%eax
 80488d9:	eb 37                	jmp    8048912 <__vprintf+0x474>
 80488db:	b8 58 9e 04 08       	mov    $0x8049e58,%eax
 80488e0:	eb 30                	jmp    8048912 <__vprintf+0x474>
 80488e2:	b8 78 9e 04 08       	mov    $0x8049e78,%eax
 80488e7:	eb 29                	jmp    8048912 <__vprintf+0x474>
 80488e9:	c7 44 24 0c b8 a0 04 	movl   $0x804a0b8,0xc(%esp)
 80488f0:	08 
 80488f1:	c7 44 24 08 98 9e 04 	movl   $0x8049e98,0x8(%esp)
 80488f8:	08 
 80488f9:	c7 44 24 04 14 01 00 	movl   $0x114,0x4(%esp)
 8048900:	00 
 8048901:	c7 04 24 02 9f 04 08 	movl   $0x8049f02,(%esp)
 8048908:	e8 af 0f 00 00       	call   80498bc <debug_panic>
 804890d:	b8 88 9e 04 08       	mov    $0x8049e88,%eax
 8048912:	8b 55 14             	mov    0x14(%ebp),%edx
 8048915:	89 54 24 10          	mov    %edx,0x10(%esp)
 8048919:	8b 4d 10             	mov    0x10(%ebp),%ecx
 804891c:	89 4c 24 0c          	mov    %ecx,0xc(%esp)
 8048920:	8d 55 dc             	lea    -0x24(%ebp),%edx
 8048923:	89 54 24 08          	mov    %edx,0x8(%esp)
 8048927:	89 44 24 04          	mov    %eax,0x4(%esp)
 804892b:	c7 04 24 00 00 00 00 	movl   $0x0,(%esp)
 8048932:	b9 00 00 00 00       	mov    $0x0,%ecx
 8048937:	8b 45 c0             	mov    -0x40(%ebp),%eax
 804893a:	8b 55 c4             	mov    -0x3c(%ebp),%edx
 804893d:	e8 4d f8 ff ff       	call   804818f <format_integer>
 8048942:	e9 e8 00 00 00       	jmp    8048a2f <__vprintf+0x591>
 8048947:	8d 5f 04             	lea    0x4(%edi),%ebx
 804894a:	8b 07                	mov    (%edi),%eax
 804894c:	88 45 ef             	mov    %al,-0x11(%ebp)
 804894f:	8b 4d 14             	mov    0x14(%ebp),%ecx
 8048952:	89 4c 24 04          	mov    %ecx,0x4(%esp)
 8048956:	8b 45 10             	mov    0x10(%ebp),%eax
 8048959:	89 04 24             	mov    %eax,(%esp)
 804895c:	8d 4d dc             	lea    -0x24(%ebp),%ecx
 804895f:	ba 01 00 00 00       	mov    $0x1,%edx
 8048964:	8d 45 ef             	lea    -0x11(%ebp),%eax
 8048967:	e8 b0 fa ff ff       	call   804841c <format_string>
 804896c:	89 df                	mov    %ebx,%edi
 804896e:	e9 bc 00 00 00       	jmp    8048a2f <__vprintf+0x591>
 8048973:	8b 1f                	mov    (%edi),%ebx
 8048975:	85 db                	test   %ebx,%ebx
 8048977:	75 05                	jne    804897e <__vprintf+0x4e0>
 8048979:	bb 11 9f 04 08       	mov    $0x8049f11,%ebx
 804897e:	83 c7 04             	add    $0x4,%edi
 8048981:	8b 45 e4             	mov    -0x1c(%ebp),%eax
 8048984:	89 44 24 04          	mov    %eax,0x4(%esp)
 8048988:	89 1c 24             	mov    %ebx,(%esp)
 804898b:	e8 81 03 00 00       	call   8048d11 <strnlen>
 8048990:	8b 55 14             	mov    0x14(%ebp),%edx
 8048993:	89 54 24 04          	mov    %edx,0x4(%esp)
 8048997:	8b 4d 10             	mov    0x10(%ebp),%ecx
 804899a:	89 0c 24             	mov    %ecx,(%esp)
 804899d:	8d 4d dc             	lea    -0x24(%ebp),%ecx
 80489a0:	89 c2                	mov    %eax,%edx
 80489a2:	89 d8                	mov    %ebx,%eax
 80489a4:	e8 73 fa ff ff       	call   804841c <format_string>
 80489a9:	e9 81 00 00 00       	jmp    8048a2f <__vprintf+0x591>
 80489ae:	8d 5f 04             	lea    0x4(%edi),%ebx
 80489b1:	8b 07                	mov    (%edi),%eax
 80489b3:	c7 45 dc 08 00 00 00 	movl   $0x8,-0x24(%ebp)
 80489ba:	ba 00 00 00 00       	mov    $0x0,%edx
 80489bf:	8b 4d 14             	mov    0x14(%ebp),%ecx
 80489c2:	89 4c 24 10          	mov    %ecx,0x10(%esp)
 80489c6:	8b 4d 10             	mov    0x10(%ebp),%ecx
 80489c9:	89 4c 24 0c          	mov    %ecx,0xc(%esp)
 80489cd:	8d 4d dc             	lea    -0x24(%ebp),%ecx
 80489d0:	89 4c 24 08          	mov    %ecx,0x8(%esp)
 80489d4:	c7 44 24 04 78 9e 04 	movl   $0x8049e78,0x4(%esp)
 80489db:	08 
 80489dc:	c7 04 24 00 00 00 00 	movl   $0x0,(%esp)
 80489e3:	b9 00 00 00 00       	mov    $0x0,%ecx
 80489e8:	e8 a2 f7 ff ff       	call   804818f <format_integer>
 80489ed:	89 df                	mov    %ebx,%edi
 80489ef:	eb 3e                	jmp    8048a2f <__vprintf+0x591>
 80489f1:	89 54 24 0c          	mov    %edx,0xc(%esp)
 80489f5:	8b 45 14             	mov    0x14(%ebp),%eax
 80489f8:	89 44 24 08          	mov    %eax,0x8(%esp)
 80489fc:	8b 55 10             	mov    0x10(%ebp),%edx
 80489ff:	89 54 24 04          	mov    %edx,0x4(%esp)
 8048a03:	c7 04 24 18 9f 04 08 	movl   $0x8049f18,(%esp)
 8048a0a:	e8 42 00 00 00       	call   8048a51 <__printf>
 8048a0f:	eb 1e                	jmp    8048a2f <__vprintf+0x591>
 8048a11:	89 54 24 0c          	mov    %edx,0xc(%esp)
 8048a15:	8b 4d 14             	mov    0x14(%ebp),%ecx
 8048a18:	89 4c 24 08          	mov    %ecx,0x8(%esp)
 8048a1c:	8b 45 10             	mov    0x10(%ebp),%eax
 8048a1f:	89 44 24 04          	mov    %eax,0x4(%esp)
 8048a23:	c7 04 24 2e 9f 04 08 	movl   $0x8049f2e,(%esp)
 8048a2a:	e8 22 00 00 00       	call   8048a51 <__printf>
 8048a2f:	83 c6 01             	add    $0x1,%esi
 8048a32:	0f b6 06             	movzbl (%esi),%eax
 8048a35:	84 c0                	test   %al,%al
 8048a37:	74 0e                	je     8048a47 <__vprintf+0x5a9>
 8048a39:	e9 7a fa ff ff       	jmp    80484b8 <__vprintf+0x1a>
 8048a3e:	83 65 dc ef          	andl   $0xffffffef,-0x24(%ebp)
 8048a42:	e9 c8 fb ff ff       	jmp    804860f <__vprintf+0x171>
 8048a47:	83 c4 5c             	add    $0x5c,%esp
 8048a4a:	5b                   	pop    %ebx
 8048a4b:	5e                   	pop    %esi
 8048a4c:	5f                   	pop    %edi
 8048a4d:	5d                   	pop    %ebp
 8048a4e:	66 90                	xchg   %ax,%ax
 8048a50:	c3                   	ret    

08048a51 <__printf>:
 8048a51:	55                   	push   %ebp
 8048a52:	89 e5                	mov    %esp,%ebp
 8048a54:	83 ec 28             	sub    $0x28,%esp
 8048a57:	8d 55 14             	lea    0x14(%ebp),%edx
 8048a5a:	89 55 fc             	mov    %edx,-0x4(%ebp)
 8048a5d:	8b 45 10             	mov    0x10(%ebp),%eax
 8048a60:	89 44 24 0c          	mov    %eax,0xc(%esp)
 8048a64:	8b 45 0c             	mov    0xc(%ebp),%eax
 8048a67:	89 44 24 08          	mov    %eax,0x8(%esp)
 8048a6b:	89 54 24 04          	mov    %edx,0x4(%esp)
 8048a6f:	8b 45 08             	mov    0x8(%ebp),%eax
 8048a72:	89 04 24             	mov    %eax,(%esp)
 8048a75:	e8 24 fa ff ff       	call   804849e <__vprintf>
 8048a7a:	c9                   	leave  
 8048a7b:	c3                   	ret    

08048a7c <vsnprintf>:
 8048a7c:	55                   	push   %ebp
 8048a7d:	89 e5                	mov    %esp,%ebp
 8048a7f:	53                   	push   %ebx
 8048a80:	83 ec 24             	sub    $0x24,%esp
 8048a83:	8b 55 0c             	mov    0xc(%ebp),%edx
 8048a86:	8b 5d 10             	mov    0x10(%ebp),%ebx
 8048a89:	8b 4d 14             	mov    0x14(%ebp),%ecx
 8048a8c:	8b 45 08             	mov    0x8(%ebp),%eax
 8048a8f:	89 45 f0             	mov    %eax,-0x10(%ebp)
 8048a92:	c7 45 f4 00 00 00 00 	movl   $0x0,-0xc(%ebp)
 8048a99:	85 d2                	test   %edx,%edx
 8048a9b:	74 29                	je     8048ac6 <vsnprintf+0x4a>
 8048a9d:	8d 42 ff             	lea    -0x1(%edx),%eax
 8048aa0:	89 45 f8             	mov    %eax,-0x8(%ebp)
 8048aa3:	8d 45 f0             	lea    -0x10(%ebp),%eax
 8048aa6:	89 44 24 0c          	mov    %eax,0xc(%esp)
 8048aaa:	c7 44 24 08 20 81 04 	movl   $0x8048120,0x8(%esp)
 8048ab1:	08 
 8048ab2:	89 4c 24 04          	mov    %ecx,0x4(%esp)
 8048ab6:	89 1c 24             	mov    %ebx,(%esp)
 8048ab9:	e8 e0 f9 ff ff       	call   804849e <__vprintf>
 8048abe:	8b 45 f0             	mov    -0x10(%ebp),%eax
 8048ac1:	c6 00 00             	movb   $0x0,(%eax)
 8048ac4:	eb 22                	jmp    8048ae8 <vsnprintf+0x6c>
 8048ac6:	c7 45 f8 00 00 00 00 	movl   $0x0,-0x8(%ebp)
 8048acd:	8d 45 f0             	lea    -0x10(%ebp),%eax
 8048ad0:	89 44 24 0c          	mov    %eax,0xc(%esp)
 8048ad4:	c7 44 24 08 20 81 04 	movl   $0x8048120,0x8(%esp)
 8048adb:	08 
 8048adc:	89 4c 24 04          	mov    %ecx,0x4(%esp)
 8048ae0:	89 1c 24             	mov    %ebx,(%esp)
 8048ae3:	e8 b6 f9 ff ff       	call   804849e <__vprintf>
 8048ae8:	8b 45 f4             	mov    -0xc(%ebp),%eax
 8048aeb:	83 c4 24             	add    $0x24,%esp
 8048aee:	5b                   	pop    %ebx
 8048aef:	5d                   	pop    %ebp
 8048af0:	c3                   	ret    

08048af1 <snprintf>:
 8048af1:	55                   	push   %ebp
 8048af2:	89 e5                	mov    %esp,%ebp
 8048af4:	83 ec 28             	sub    $0x28,%esp
 8048af7:	8d 45 14             	lea    0x14(%ebp),%eax
 8048afa:	89 45 fc             	mov    %eax,-0x4(%ebp)
 8048afd:	89 44 24 0c          	mov    %eax,0xc(%esp)
 8048b01:	8b 45 10             	mov    0x10(%ebp),%eax
 8048b04:	89 44 24 08          	mov    %eax,0x8(%esp)
 8048b08:	8b 45 0c             	mov    0xc(%ebp),%eax
 8048b0b:	89 44 24 04          	mov    %eax,0x4(%esp)
 8048b0f:	8b 45 08             	mov    0x8(%ebp),%eax
 8048b12:	89 04 24             	mov    %eax,(%esp)
 8048b15:	e8 62 ff ff ff       	call   8048a7c <vsnprintf>
 8048b1a:	c9                   	leave  
 8048b1b:	c3                   	ret    

08048b1c <printf>:
 8048b1c:	55                   	push   %ebp
 8048b1d:	89 e5                	mov    %esp,%ebp
 8048b1f:	83 ec 18             	sub    $0x18,%esp
 8048b22:	8d 45 0c             	lea    0xc(%ebp),%eax
 8048b25:	89 45 fc             	mov    %eax,-0x4(%ebp)
 8048b28:	89 44 24 04          	mov    %eax,0x4(%esp)
 8048b2c:	8b 45 08             	mov    0x8(%ebp),%eax
 8048b2f:	89 04 24             	mov    %eax,(%esp)
 8048b32:	e8 4e 10 00 00       	call   8049b85 <vprintf>
 8048b37:	c9                   	leave  
 8048b38:	c3                   	ret    

08048b39 <hex_dump>:
 8048b39:	55                   	push   %ebp
 8048b3a:	89 e5                	mov    %esp,%ebp
 8048b3c:	57                   	push   %edi
 8048b3d:	56                   	push   %esi
 8048b3e:	53                   	push   %ebx
 8048b3f:	83 ec 1c             	sub    $0x1c,%esp
 8048b42:	0f b6 45 14          	movzbl 0x14(%ebp),%eax
 8048b46:	88 45 e7             	mov    %al,-0x19(%ebp)
 8048b49:	83 7d 10 00          	cmpl   $0x0,0x10(%ebp)
 8048b4d:	0f 84 7e 01 00 00    	je     8048cd1 <hex_dump+0x198>
 8048b53:	8b 55 0c             	mov    0xc(%ebp),%edx
 8048b56:	89 55 e8             	mov    %edx,-0x18(%ebp)
 8048b59:	8b 7d 08             	mov    0x8(%ebp),%edi
 8048b5c:	83 e7 0f             	and    $0xf,%edi
 8048b5f:	b8 10 00 00 00       	mov    $0x10,%eax
 8048b64:	29 f8                	sub    %edi,%eax
 8048b66:	c7 45 ec 10 00 00 00 	movl   $0x10,-0x14(%ebp)
 8048b6d:	3b 45 10             	cmp    0x10(%ebp),%eax
 8048b70:	76 08                	jbe    8048b7a <hex_dump+0x41>
 8048b72:	8b 45 10             	mov    0x10(%ebp),%eax
 8048b75:	01 f8                	add    %edi,%eax
 8048b77:	89 45 ec             	mov    %eax,-0x14(%ebp)
 8048b7a:	8b 55 ec             	mov    -0x14(%ebp),%edx
 8048b7d:	29 fa                	sub    %edi,%edx
 8048b7f:	89 55 f0             	mov    %edx,-0x10(%ebp)
 8048b82:	8b 45 08             	mov    0x8(%ebp),%eax
 8048b85:	83 e0 f0             	and    $0xfffffff0,%eax
 8048b88:	89 44 24 04          	mov    %eax,0x4(%esp)
 8048b8c:	c7 44 24 08 00 00 00 	movl   $0x0,0x8(%esp)
 8048b93:	00 
 8048b94:	c7 04 24 45 9f 04 08 	movl   $0x8049f45,(%esp)
 8048b9b:	e8 7c ff ff ff       	call   8048b1c <printf>
 8048ba0:	be 00 00 00 00       	mov    $0x0,%esi
 8048ba5:	85 ff                	test   %edi,%edi
 8048ba7:	74 1a                	je     8048bc3 <hex_dump+0x8a>
 8048ba9:	bb 00 00 00 00       	mov    $0x0,%ebx
 8048bae:	c7 04 24 4d 9f 04 08 	movl   $0x8049f4d,(%esp)
 8048bb5:	e8 62 ff ff ff       	call   8048b1c <printf>
 8048bba:	83 c3 01             	add    $0x1,%ebx
 8048bbd:	39 fb                	cmp    %edi,%ebx
 8048bbf:	75 ed                	jne    8048bae <hex_dump+0x75>
 8048bc1:	89 fe                	mov    %edi,%esi
 8048bc3:	39 75 ec             	cmp    %esi,-0x14(%ebp)
 8048bc6:	76 41                	jbe    8048c09 <hex_dump+0xd0>
 8048bc8:	89 f0                	mov    %esi,%eax
 8048bca:	29 f8                	sub    %edi,%eax
 8048bcc:	8b 55 e8             	mov    -0x18(%ebp),%edx
 8048bcf:	8d 1c 10             	lea    (%eax,%edx,1),%ebx
 8048bd2:	83 fe 07             	cmp    $0x7,%esi
 8048bd5:	0f 94 c0             	sete   %al
 8048bd8:	0f b6 c0             	movzbl %al,%eax
 8048bdb:	83 e8 01             	sub    $0x1,%eax
 8048bde:	83 e0 f3             	and    $0xfffffff3,%eax
 8048be1:	83 c0 2d             	add    $0x2d,%eax
 8048be4:	89 44 24 08          	mov    %eax,0x8(%esp)
 8048be8:	0f b6 03             	movzbl (%ebx),%eax
 8048beb:	89 44 24 04          	mov    %eax,0x4(%esp)
 8048bef:	c7 04 24 51 9f 04 08 	movl   $0x8049f51,(%esp)
 8048bf6:	e8 21 ff ff ff       	call   8048b1c <printf>
 8048bfb:	83 c6 01             	add    $0x1,%esi
 8048bfe:	83 c3 01             	add    $0x1,%ebx
 8048c01:	3b 75 ec             	cmp    -0x14(%ebp),%esi
 8048c04:	75 cc                	jne    8048bd2 <hex_dump+0x99>
 8048c06:	8b 75 ec             	mov    -0x14(%ebp),%esi
 8048c09:	80 7d e7 00          	cmpb   $0x0,-0x19(%ebp)
 8048c0d:	0f 84 9f 00 00 00    	je     8048cb2 <hex_dump+0x179>
 8048c13:	83 fe 0f             	cmp    $0xf,%esi
 8048c16:	77 14                	ja     8048c2c <hex_dump+0xf3>
 8048c18:	c7 04 24 4d 9f 04 08 	movl   $0x8049f4d,(%esp)
 8048c1f:	e8 f8 fe ff ff       	call   8048b1c <printf>
 8048c24:	83 c6 01             	add    $0x1,%esi
 8048c27:	83 fe 10             	cmp    $0x10,%esi
 8048c2a:	75 ec                	jne    8048c18 <hex_dump+0xdf>
 8048c2c:	c7 04 24 7c 00 00 00 	movl   $0x7c,(%esp)
 8048c33:	e8 6f 0f 00 00       	call   8049ba7 <putchar>
 8048c38:	be 00 00 00 00       	mov    $0x0,%esi
 8048c3d:	85 ff                	test   %edi,%edi
 8048c3f:	74 1a                	je     8048c5b <hex_dump+0x122>
 8048c41:	bb 00 00 00 00       	mov    $0x0,%ebx
 8048c46:	c7 04 24 20 00 00 00 	movl   $0x20,(%esp)
 8048c4d:	e8 55 0f 00 00       	call   8049ba7 <putchar>
 8048c52:	83 c3 01             	add    $0x1,%ebx
 8048c55:	39 fb                	cmp    %edi,%ebx
 8048c57:	75 ed                	jne    8048c46 <hex_dump+0x10d>
 8048c59:	89 fe                	mov    %edi,%esi
 8048c5b:	39 75 ec             	cmp    %esi,-0x14(%ebp)
 8048c5e:	76 2d                	jbe    8048c8d <hex_dump+0x154>
 8048c60:	89 f0                	mov    %esi,%eax
 8048c62:	29 f8                	sub    %edi,%eax
 8048c64:	8b 55 e8             	mov    -0x18(%ebp),%edx
 8048c67:	8d 1c 10             	lea    (%eax,%edx,1),%ebx
 8048c6a:	0f b6 13             	movzbl (%ebx),%edx
 8048c6d:	8d 42 e0             	lea    -0x20(%edx),%eax
 8048c70:	83 f8 5e             	cmp    $0x5e,%eax
 8048c73:	76 05                	jbe    8048c7a <hex_dump+0x141>
 8048c75:	ba 2e 00 00 00       	mov    $0x2e,%edx
 8048c7a:	89 14 24             	mov    %edx,(%esp)
 8048c7d:	e8 25 0f 00 00       	call   8049ba7 <putchar>
 8048c82:	83 c6 01             	add    $0x1,%esi
 8048c85:	83 c3 01             	add    $0x1,%ebx
 8048c88:	3b 75 ec             	cmp    -0x14(%ebp),%esi
 8048c8b:	75 dd                	jne    8048c6a <hex_dump+0x131>
 8048c8d:	83 fe 0f             	cmp    $0xf,%esi
 8048c90:	77 14                	ja     8048ca6 <hex_dump+0x16d>
 8048c92:	c7 04 24 20 00 00 00 	movl   $0x20,(%esp)
 8048c99:	e8 09 0f 00 00       	call   8049ba7 <putchar>
 8048c9e:	83 c6 01             	add    $0x1,%esi
 8048ca1:	83 fe 10             	cmp    $0x10,%esi
 8048ca4:	75 ec                	jne    8048c92 <hex_dump+0x159>
 8048ca6:	c7 04 24 7c 00 00 00 	movl   $0x7c,(%esp)
 8048cad:	e8 f5 0e 00 00       	call   8049ba7 <putchar>
 8048cb2:	c7 04 24 0a 00 00 00 	movl   $0xa,(%esp)
 8048cb9:	e8 e9 0e 00 00       	call   8049ba7 <putchar>
 8048cbe:	8b 45 f0             	mov    -0x10(%ebp),%eax
 8048cc1:	29 45 10             	sub    %eax,0x10(%ebp)
 8048cc4:	74 0b                	je     8048cd1 <hex_dump+0x198>
 8048cc6:	01 45 08             	add    %eax,0x8(%ebp)
 8048cc9:	01 45 e8             	add    %eax,-0x18(%ebp)
 8048ccc:	e9 88 fe ff ff       	jmp    8048b59 <hex_dump+0x20>
 8048cd1:	83 c4 1c             	add    $0x1c,%esp
 8048cd4:	5b                   	pop    %ebx
 8048cd5:	5e                   	pop    %esi
 8048cd6:	5f                   	pop    %edi
 8048cd7:	5d                   	pop    %ebp
 8048cd8:	c3                   	ret    
 8048cd9:	90                   	nop    
 8048cda:	90                   	nop    
 8048cdb:	90                   	nop    
 8048cdc:	90                   	nop    
 8048cdd:	90                   	nop    
 8048cde:	90                   	nop    
 8048cdf:	90                   	nop    

08048ce0 <strrchr>:
 8048ce0:	55                   	push   %ebp
 8048ce1:	89 e5                	mov    %esp,%ebp
 8048ce3:	53                   	push   %ebx
 8048ce4:	8b 4d 08             	mov    0x8(%ebp),%ecx
 8048ce7:	0f b6 45 0c          	movzbl 0xc(%ebp),%eax
 8048ceb:	0f b6 11             	movzbl (%ecx),%edx
 8048cee:	bb 00 00 00 00       	mov    $0x0,%ebx
 8048cf3:	84 d2                	test   %dl,%dl
 8048cf5:	74 15                	je     8048d0c <strrchr+0x2c>
 8048cf7:	bb 00 00 00 00       	mov    $0x0,%ebx
 8048cfc:	38 d0                	cmp    %dl,%al
 8048cfe:	75 02                	jne    8048d02 <strrchr+0x22>
 8048d00:	89 cb                	mov    %ecx,%ebx
 8048d02:	83 c1 01             	add    $0x1,%ecx
 8048d05:	0f b6 11             	movzbl (%ecx),%edx
 8048d08:	84 d2                	test   %dl,%dl
 8048d0a:	75 f0                	jne    8048cfc <strrchr+0x1c>
 8048d0c:	89 d8                	mov    %ebx,%eax
 8048d0e:	5b                   	pop    %ebx
 8048d0f:	5d                   	pop    %ebp
 8048d10:	c3                   	ret    

08048d11 <strnlen>:
 8048d11:	55                   	push   %ebp
 8048d12:	89 e5                	mov    %esp,%ebp
 8048d14:	8b 55 08             	mov    0x8(%ebp),%edx
 8048d17:	8b 4d 0c             	mov    0xc(%ebp),%ecx
 8048d1a:	80 3a 00             	cmpb   $0x0,(%edx)
 8048d1d:	74 09                	je     8048d28 <strnlen+0x17>
 8048d1f:	b8 00 00 00 00       	mov    $0x0,%eax
 8048d24:	85 c9                	test   %ecx,%ecx
 8048d26:	75 07                	jne    8048d2f <strnlen+0x1e>
 8048d28:	b8 00 00 00 00       	mov    $0x0,%eax
 8048d2d:	eb 0d                	jmp    8048d3c <strnlen+0x2b>
 8048d2f:	83 c0 01             	add    $0x1,%eax
 8048d32:	80 3c 10 00          	cmpb   $0x0,(%eax,%edx,1)
 8048d36:	74 04                	je     8048d3c <strnlen+0x2b>
 8048d38:	39 c1                	cmp    %eax,%ecx
 8048d3a:	77 f3                	ja     8048d2f <strnlen+0x1e>
 8048d3c:	5d                   	pop    %ebp
 8048d3d:	8d 76 00             	lea    0x0(%esi),%esi
 8048d40:	c3                   	ret    

08048d41 <strlen>:
 8048d41:	55                   	push   %ebp
 8048d42:	89 e5                	mov    %esp,%ebp
 8048d44:	83 ec 18             	sub    $0x18,%esp
 8048d47:	8b 55 08             	mov    0x8(%ebp),%edx
 8048d4a:	85 d2                	test   %edx,%edx
 8048d4c:	74 09                	je     8048d57 <strlen+0x16>
 8048d4e:	89 d0                	mov    %edx,%eax
 8048d50:	80 3a 00             	cmpb   $0x0,(%edx)
 8048d53:	74 38                	je     8048d8d <strlen+0x4c>
 8048d55:	eb 2c                	jmp    8048d83 <strlen+0x42>
 8048d57:	c7 44 24 10 90 9f 04 	movl   $0x8049f90,0x10(%esp)
 8048d5e:	08 
 8048d5f:	c7 44 24 0c 9f 9f 04 	movl   $0x8049f9f,0xc(%esp)
 8048d66:	08 
 8048d67:	c7 44 24 08 b2 9e 04 	movl   $0x8049eb2,0x8(%esp)
 8048d6e:	08 
 8048d6f:	c7 44 24 04 29 01 00 	movl   $0x129,0x4(%esp)
 8048d76:	00 
 8048d77:	c7 04 24 b6 9f 04 08 	movl   $0x8049fb6,(%esp)
 8048d7e:	e8 39 0b 00 00       	call   80498bc <debug_panic>
 8048d83:	89 d0                	mov    %edx,%eax
 8048d85:	83 c0 01             	add    $0x1,%eax
 8048d88:	80 38 00             	cmpb   $0x0,(%eax)
 8048d8b:	75 f8                	jne    8048d85 <strlen+0x44>
 8048d8d:	29 d0                	sub    %edx,%eax
 8048d8f:	c9                   	leave  
 8048d90:	c3                   	ret    

08048d91 <memset>:
 8048d91:	55                   	push   %ebp
 8048d92:	89 e5                	mov    %esp,%ebp
 8048d94:	53                   	push   %ebx
 8048d95:	83 ec 14             	sub    $0x14,%esp
 8048d98:	8b 45 08             	mov    0x8(%ebp),%eax
 8048d9b:	8b 5d 0c             	mov    0xc(%ebp),%ebx
 8048d9e:	8b 4d 10             	mov    0x10(%ebp),%ecx
 8048da1:	85 c0                	test   %eax,%eax
 8048da3:	75 04                	jne    8048da9 <memset+0x18>
 8048da5:	85 c9                	test   %ecx,%ecx
 8048da7:	75 0b                	jne    8048db4 <memset+0x23>
 8048da9:	ba 00 00 00 00       	mov    $0x0,%edx
 8048dae:	85 c9                	test   %ecx,%ecx
 8048db0:	75 2e                	jne    8048de0 <memset+0x4f>
 8048db2:	eb 36                	jmp    8048dea <memset+0x59>
 8048db4:	c7 44 24 10 c6 9f 04 	movl   $0x8049fc6,0x10(%esp)
 8048dbb:	08 
 8048dbc:	c7 44 24 0c 9f 9f 04 	movl   $0x8049f9f,0xc(%esp)
 8048dc3:	08 
 8048dc4:	c7 44 24 08 b9 9e 04 	movl   $0x8049eb9,0x8(%esp)
 8048dcb:	08 
 8048dcc:	c7 44 24 04 1b 01 00 	movl   $0x11b,0x4(%esp)
 8048dd3:	00 
 8048dd4:	c7 04 24 b6 9f 04 08 	movl   $0x8049fb6,(%esp)
 8048ddb:	e8 dc 0a 00 00       	call   80498bc <debug_panic>
 8048de0:	88 1c 02             	mov    %bl,(%edx,%eax,1)
 8048de3:	83 c2 01             	add    $0x1,%edx
 8048de6:	39 ca                	cmp    %ecx,%edx
 8048de8:	75 f6                	jne    8048de0 <memset+0x4f>
 8048dea:	83 c4 14             	add    $0x14,%esp
 8048ded:	5b                   	pop    %ebx
 8048dee:	5d                   	pop    %ebp
 8048def:	c3                   	ret    

08048df0 <strchr>:
 8048df0:	55                   	push   %ebp
 8048df1:	89 e5                	mov    %esp,%ebp
 8048df3:	83 ec 18             	sub    $0x18,%esp
 8048df6:	8b 45 08             	mov    0x8(%ebp),%eax
 8048df9:	85 c0                	test   %eax,%eax
 8048dfb:	74 0d                	je     8048e0a <strchr+0x1a>
 8048dfd:	0f b6 4d 0c          	movzbl 0xc(%ebp),%ecx
 8048e01:	0f b6 10             	movzbl (%eax),%edx
 8048e04:	38 ca                	cmp    %cl,%dl
 8048e06:	75 2e                	jne    8048e36 <strchr+0x46>
 8048e08:	eb 49                	jmp    8048e53 <strchr+0x63>
 8048e0a:	c7 44 24 10 90 9f 04 	movl   $0x8049f90,0x10(%esp)
 8048e11:	08 
 8048e12:	c7 44 24 0c 9f 9f 04 	movl   $0x8049f9f,0xc(%esp)
 8048e19:	08 
 8048e1a:	c7 44 24 08 c9 9e 04 	movl   $0x8049ec9,0x8(%esp)
 8048e21:	08 
 8048e22:	c7 44 24 04 75 00 00 	movl   $0x75,0x4(%esp)
 8048e29:	00 
 8048e2a:	c7 04 24 b6 9f 04 08 	movl   $0x8049fb6,(%esp)
 8048e31:	e8 86 0a 00 00       	call   80498bc <debug_panic>
 8048e36:	84 d2                	test   %dl,%dl
 8048e38:	75 08                	jne    8048e42 <strchr+0x52>
 8048e3a:	eb 12                	jmp    8048e4e <strchr+0x5e>
 8048e3c:	84 d2                	test   %dl,%dl
 8048e3e:	66 90                	xchg   %ax,%ax
 8048e40:	74 0c                	je     8048e4e <strchr+0x5e>
 8048e42:	83 c0 01             	add    $0x1,%eax
 8048e45:	0f b6 10             	movzbl (%eax),%edx
 8048e48:	38 ca                	cmp    %cl,%dl
 8048e4a:	75 f0                	jne    8048e3c <strchr+0x4c>
 8048e4c:	eb 05                	jmp    8048e53 <strchr+0x63>
 8048e4e:	b8 00 00 00 00       	mov    $0x0,%eax
 8048e53:	c9                   	leave  
 8048e54:	c3                   	ret    

08048e55 <strtok_r>:
 8048e55:	55                   	push   %ebp
 8048e56:	89 e5                	mov    %esp,%ebp
 8048e58:	57                   	push   %edi
 8048e59:	56                   	push   %esi
 8048e5a:	53                   	push   %ebx
 8048e5b:	83 ec 1c             	sub    $0x1c,%esp
 8048e5e:	8b 75 08             	mov    0x8(%ebp),%esi
 8048e61:	83 7d 0c 00          	cmpl   $0x0,0xc(%ebp)
 8048e65:	75 2c                	jne    8048e93 <strtok_r+0x3e>
 8048e67:	c7 44 24 10 df 9f 04 	movl   $0x8049fdf,0x10(%esp)
 8048e6e:	08 
 8048e6f:	c7 44 24 0c 9f 9f 04 	movl   $0x8049f9f,0xc(%esp)
 8048e76:	08 
 8048e77:	c7 44 24 08 c0 9e 04 	movl   $0x8049ec0,0x8(%esp)
 8048e7e:	08 
 8048e7f:	c7 44 24 04 ef 00 00 	movl   $0xef,0x4(%esp)
 8048e86:	00 
 8048e87:	c7 04 24 b6 9f 04 08 	movl   $0x8049fb6,(%esp)
 8048e8e:	e8 29 0a 00 00       	call   80498bc <debug_panic>
 8048e93:	83 7d 10 00          	cmpl   $0x0,0x10(%ebp)
 8048e97:	75 2c                	jne    8048ec5 <strtok_r+0x70>
 8048e99:	c7 44 24 10 f2 9f 04 	movl   $0x8049ff2,0x10(%esp)
 8048ea0:	08 
 8048ea1:	c7 44 24 0c 9f 9f 04 	movl   $0x8049f9f,0xc(%esp)
 8048ea8:	08 
 8048ea9:	c7 44 24 08 c0 9e 04 	movl   $0x8049ec0,0x8(%esp)
 8048eb0:	08 
 8048eb1:	c7 44 24 04 f0 00 00 	movl   $0xf0,0x4(%esp)
 8048eb8:	00 
 8048eb9:	c7 04 24 b6 9f 04 08 	movl   $0x8049fb6,(%esp)
 8048ec0:	e8 f7 09 00 00       	call   80498bc <debug_panic>
 8048ec5:	85 f6                	test   %esi,%esi
 8048ec7:	75 48                	jne    8048f11 <strtok_r+0xbc>
 8048ec9:	8b 45 10             	mov    0x10(%ebp),%eax
 8048ecc:	8b 30                	mov    (%eax),%esi
 8048ece:	85 f6                	test   %esi,%esi
 8048ed0:	75 3f                	jne    8048f11 <strtok_r+0xbc>
 8048ed2:	c7 44 24 10 e8 9f 04 	movl   $0x8049fe8,0x10(%esp)
 8048ed9:	08 
 8048eda:	c7 44 24 0c 9f 9f 04 	movl   $0x8049f9f,0xc(%esp)
 8048ee1:	08 
 8048ee2:	c7 44 24 08 c0 9e 04 	movl   $0x8049ec0,0x8(%esp)
 8048ee9:	08 
 8048eea:	c7 44 24 04 f6 00 00 	movl   $0xf6,0x4(%esp)
 8048ef1:	00 
 8048ef2:	c7 04 24 b6 9f 04 08 	movl   $0x8049fb6,(%esp)
 8048ef9:	e8 be 09 00 00       	call   80498bc <debug_panic>
 8048efe:	84 db                	test   %bl,%bl
 8048f00:	75 0c                	jne    8048f0e <strtok_r+0xb9>
 8048f02:	8b 55 10             	mov    0x10(%ebp),%edx
 8048f05:	89 32                	mov    %esi,(%edx)
 8048f07:	b8 00 00 00 00       	mov    $0x0,%eax
 8048f0c:	eb 58                	jmp    8048f66 <strtok_r+0x111>
 8048f0e:	83 c6 01             	add    $0x1,%esi
 8048f11:	0f b6 1e             	movzbl (%esi),%ebx
 8048f14:	0f be c3             	movsbl %bl,%eax
 8048f17:	89 44 24 04          	mov    %eax,0x4(%esp)
 8048f1b:	8b 45 0c             	mov    0xc(%ebp),%eax
 8048f1e:	89 04 24             	mov    %eax,(%esp)
 8048f21:	e8 ca fe ff ff       	call   8048df0 <strchr>
 8048f26:	85 c0                	test   %eax,%eax
 8048f28:	75 d4                	jne    8048efe <strtok_r+0xa9>
 8048f2a:	89 f3                	mov    %esi,%ebx
 8048f2c:	83 c3 01             	add    $0x1,%ebx
 8048f2f:	0f b6 3b             	movzbl (%ebx),%edi
 8048f32:	89 fa                	mov    %edi,%edx
 8048f34:	0f be c2             	movsbl %dl,%eax
 8048f37:	89 44 24 04          	mov    %eax,0x4(%esp)
 8048f3b:	8b 45 0c             	mov    0xc(%ebp),%eax
 8048f3e:	89 04 24             	mov    %eax,(%esp)
 8048f41:	e8 aa fe ff ff       	call   8048df0 <strchr>
 8048f46:	85 c0                	test   %eax,%eax
 8048f48:	74 e2                	je     8048f2c <strtok_r+0xd7>
 8048f4a:	89 fa                	mov    %edi,%edx
 8048f4c:	84 d2                	test   %dl,%dl
 8048f4e:	74 0f                	je     8048f5f <strtok_r+0x10a>
 8048f50:	c6 03 00             	movb   $0x0,(%ebx)
 8048f53:	8d 43 01             	lea    0x1(%ebx),%eax
 8048f56:	8b 55 10             	mov    0x10(%ebp),%edx
 8048f59:	89 02                	mov    %eax,(%edx)
 8048f5b:	89 f0                	mov    %esi,%eax
 8048f5d:	eb 07                	jmp    8048f66 <strtok_r+0x111>
 8048f5f:	8b 45 10             	mov    0x10(%ebp),%eax
 8048f62:	89 18                	mov    %ebx,(%eax)
 8048f64:	89 f0                	mov    %esi,%eax
 8048f66:	83 c4 1c             	add    $0x1c,%esp
 8048f69:	5b                   	pop    %ebx
 8048f6a:	5e                   	pop    %esi
 8048f6b:	5f                   	pop    %edi
 8048f6c:	5d                   	pop    %ebp
 8048f6d:	c3                   	ret    

08048f6e <strspn>:
 8048f6e:	55                   	push   %ebp
 8048f6f:	89 e5                	mov    %esp,%ebp
 8048f71:	57                   	push   %edi
 8048f72:	56                   	push   %esi
 8048f73:	53                   	push   %ebx
 8048f74:	83 ec 0c             	sub    $0xc,%esp
 8048f77:	8b 7d 08             	mov    0x8(%ebp),%edi
 8048f7a:	8b 75 0c             	mov    0xc(%ebp),%esi
 8048f7d:	0f b6 07             	movzbl (%edi),%eax
 8048f80:	bb 00 00 00 00       	mov    $0x0,%ebx
 8048f85:	84 c0                	test   %al,%al
 8048f87:	74 23                	je     8048fac <strspn+0x3e>
 8048f89:	bb 00 00 00 00       	mov    $0x0,%ebx
 8048f8e:	0f be c0             	movsbl %al,%eax
 8048f91:	89 44 24 04          	mov    %eax,0x4(%esp)
 8048f95:	89 34 24             	mov    %esi,(%esp)
 8048f98:	e8 53 fe ff ff       	call   8048df0 <strchr>
 8048f9d:	85 c0                	test   %eax,%eax
 8048f9f:	74 0b                	je     8048fac <strspn+0x3e>
 8048fa1:	83 c3 01             	add    $0x1,%ebx
 8048fa4:	0f b6 04 3b          	movzbl (%ebx,%edi,1),%eax
 8048fa8:	84 c0                	test   %al,%al
 8048faa:	75 e2                	jne    8048f8e <strspn+0x20>
 8048fac:	89 d8                	mov    %ebx,%eax
 8048fae:	83 c4 0c             	add    $0xc,%esp
 8048fb1:	5b                   	pop    %ebx
 8048fb2:	5e                   	pop    %esi
 8048fb3:	5f                   	pop    %edi
 8048fb4:	5d                   	pop    %ebp
 8048fb5:	c3                   	ret    

08048fb6 <strpbrk>:
 8048fb6:	55                   	push   %ebp
 8048fb7:	89 e5                	mov    %esp,%ebp
 8048fb9:	56                   	push   %esi
 8048fba:	53                   	push   %ebx
 8048fbb:	83 ec 10             	sub    $0x10,%esp
 8048fbe:	8b 5d 08             	mov    0x8(%ebp),%ebx
 8048fc1:	8b 75 0c             	mov    0xc(%ebp),%esi
 8048fc4:	0f b6 03             	movzbl (%ebx),%eax
 8048fc7:	84 c0                	test   %al,%al
 8048fc9:	74 21                	je     8048fec <strpbrk+0x36>
 8048fcb:	0f be c0             	movsbl %al,%eax
 8048fce:	89 44 24 04          	mov    %eax,0x4(%esp)
 8048fd2:	89 34 24             	mov    %esi,(%esp)
 8048fd5:	e8 16 fe ff ff       	call   8048df0 <strchr>
 8048fda:	85 c0                	test   %eax,%eax
 8048fdc:	74 04                	je     8048fe2 <strpbrk+0x2c>
 8048fde:	89 d8                	mov    %ebx,%eax
 8048fe0:	eb 0f                	jmp    8048ff1 <strpbrk+0x3b>
 8048fe2:	83 c3 01             	add    $0x1,%ebx
 8048fe5:	0f b6 03             	movzbl (%ebx),%eax
 8048fe8:	84 c0                	test   %al,%al
 8048fea:	75 df                	jne    8048fcb <strpbrk+0x15>
 8048fec:	b8 00 00 00 00       	mov    $0x0,%eax
 8048ff1:	83 c4 10             	add    $0x10,%esp
 8048ff4:	5b                   	pop    %ebx
 8048ff5:	5e                   	pop    %esi
 8048ff6:	5d                   	pop    %ebp
 8048ff7:	c3                   	ret    

08048ff8 <strcspn>:
 8048ff8:	55                   	push   %ebp
 8048ff9:	89 e5                	mov    %esp,%ebp
 8048ffb:	57                   	push   %edi
 8048ffc:	56                   	push   %esi
 8048ffd:	53                   	push   %ebx
 8048ffe:	83 ec 0c             	sub    $0xc,%esp
 8049001:	8b 7d 08             	mov    0x8(%ebp),%edi
 8049004:	8b 75 0c             	mov    0xc(%ebp),%esi
 8049007:	0f b6 07             	movzbl (%edi),%eax
 804900a:	bb 00 00 00 00       	mov    $0x0,%ebx
 804900f:	84 c0                	test   %al,%al
 8049011:	74 23                	je     8049036 <strcspn+0x3e>
 8049013:	bb 00 00 00 00       	mov    $0x0,%ebx
 8049018:	0f be c0             	movsbl %al,%eax
 804901b:	89 44 24 04          	mov    %eax,0x4(%esp)
 804901f:	89 34 24             	mov    %esi,(%esp)
 8049022:	e8 c9 fd ff ff       	call   8048df0 <strchr>
 8049027:	85 c0                	test   %eax,%eax
 8049029:	75 0b                	jne    8049036 <strcspn+0x3e>
 804902b:	83 c3 01             	add    $0x1,%ebx
 804902e:	0f b6 04 3b          	movzbl (%ebx,%edi,1),%eax
 8049032:	84 c0                	test   %al,%al
 8049034:	75 e2                	jne    8049018 <strcspn+0x20>
 8049036:	89 d8                	mov    %ebx,%eax
 8049038:	83 c4 0c             	add    $0xc,%esp
 804903b:	5b                   	pop    %ebx
 804903c:	5e                   	pop    %esi
 804903d:	5f                   	pop    %edi
 804903e:	5d                   	pop    %ebp
 804903f:	c3                   	ret    

08049040 <memchr>:
 8049040:	55                   	push   %ebp
 8049041:	89 e5                	mov    %esp,%ebp
 8049043:	83 ec 18             	sub    $0x18,%esp
 8049046:	8b 45 08             	mov    0x8(%ebp),%eax
 8049049:	85 c0                	test   %eax,%eax
 804904b:	75 06                	jne    8049053 <memchr+0x13>
 804904d:	83 7d 10 00          	cmpl   $0x0,0x10(%ebp)
 8049051:	75 15                	jne    8049068 <memchr+0x28>
 8049053:	8b 55 10             	mov    0x10(%ebp),%edx
 8049056:	83 ea 01             	sub    $0x1,%edx
 8049059:	83 fa ff             	cmp    $0xffffffff,%edx
 804905c:	74 45                	je     80490a3 <memchr+0x63>
 804905e:	0f b6 4d 0c          	movzbl 0xc(%ebp),%ecx
 8049062:	38 08                	cmp    %cl,(%eax)
 8049064:	75 35                	jne    804909b <memchr+0x5b>
 8049066:	eb 40                	jmp    80490a8 <memchr+0x68>
 8049068:	c7 44 24 10 03 a0 04 	movl   $0x804a003,0x10(%esp)
 804906f:	08 
 8049070:	c7 44 24 0c 9f 9f 04 	movl   $0x8049f9f,0xc(%esp)
 8049077:	08 
 8049078:	c7 44 24 08 d0 9e 04 	movl   $0x8049ed0,0x8(%esp)
 804907f:	08 
 8049080:	c7 44 24 04 63 00 00 	movl   $0x63,0x4(%esp)
 8049087:	00 
 8049088:	c7 04 24 b6 9f 04 08 	movl   $0x8049fb6,(%esp)
 804908f:	e8 28 08 00 00       	call   80498bc <debug_panic>
 8049094:	83 c0 01             	add    $0x1,%eax
 8049097:	38 08                	cmp    %cl,(%eax)
 8049099:	74 0d                	je     80490a8 <memchr+0x68>
 804909b:	83 ea 01             	sub    $0x1,%edx
 804909e:	83 fa ff             	cmp    $0xffffffff,%edx
 80490a1:	75 f1                	jne    8049094 <memchr+0x54>
 80490a3:	b8 00 00 00 00       	mov    $0x0,%eax
 80490a8:	c9                   	leave  
 80490a9:	c3                   	ret    

080490aa <strcmp>:
 80490aa:	55                   	push   %ebp
 80490ab:	89 e5                	mov    %esp,%ebp
 80490ad:	83 ec 18             	sub    $0x18,%esp
 80490b0:	8b 45 08             	mov    0x8(%ebp),%eax
 80490b3:	8b 55 0c             	mov    0xc(%ebp),%edx
 80490b6:	85 c0                	test   %eax,%eax
 80490b8:	75 2c                	jne    80490e6 <strcmp+0x3c>
 80490ba:	c7 44 24 10 1e a0 04 	movl   $0x804a01e,0x10(%esp)
 80490c1:	08 
 80490c2:	c7 44 24 0c 9f 9f 04 	movl   $0x8049f9f,0xc(%esp)
 80490c9:	08 
 80490ca:	c7 44 24 08 d7 9e 04 	movl   $0x8049ed7,0x8(%esp)
 80490d1:	08 
 80490d2:	c7 44 24 04 4e 00 00 	movl   $0x4e,0x4(%esp)
 80490d9:	00 
 80490da:	c7 04 24 b6 9f 04 08 	movl   $0x8049fb6,(%esp)
 80490e1:	e8 d6 07 00 00       	call   80498bc <debug_panic>
 80490e6:	85 d2                	test   %edx,%edx
 80490e8:	74 0f                	je     80490f9 <strcmp+0x4f>
 80490ea:	89 d1                	mov    %edx,%ecx
 80490ec:	0f b6 10             	movzbl (%eax),%edx
 80490ef:	84 d2                	test   %dl,%dl
 80490f1:	74 44                	je     8049137 <strcmp+0x8d>
 80490f3:	3a 11                	cmp    (%ecx),%dl
 80490f5:	74 2e                	je     8049125 <strcmp+0x7b>
 80490f7:	eb 3e                	jmp    8049137 <strcmp+0x8d>
 80490f9:	c7 44 24 10 28 a0 04 	movl   $0x804a028,0x10(%esp)
 8049100:	08 
 8049101:	c7 44 24 0c 9f 9f 04 	movl   $0x8049f9f,0xc(%esp)
 8049108:	08 
 8049109:	c7 44 24 08 d7 9e 04 	movl   $0x8049ed7,0x8(%esp)
 8049110:	08 
 8049111:	c7 44 24 04 4f 00 00 	movl   $0x4f,0x4(%esp)
 8049118:	00 
 8049119:	c7 04 24 b6 9f 04 08 	movl   $0x8049fb6,(%esp)
 8049120:	e8 97 07 00 00       	call   80498bc <debug_panic>
 8049125:	83 c1 01             	add    $0x1,%ecx
 8049128:	0f b6 50 01          	movzbl 0x1(%eax),%edx
 804912c:	84 d2                	test   %dl,%dl
 804912e:	74 07                	je     8049137 <strcmp+0x8d>
 8049130:	83 c0 01             	add    $0x1,%eax
 8049133:	3a 11                	cmp    (%ecx),%dl
 8049135:	74 ee                	je     8049125 <strcmp+0x7b>
 8049137:	0f b6 09             	movzbl (%ecx),%ecx
 804913a:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
 804913f:	38 d1                	cmp    %dl,%cl
 8049141:	77 08                	ja     804914b <strcmp+0xa1>
 8049143:	38 ca                	cmp    %cl,%dl
 8049145:	0f 97 c0             	seta   %al
 8049148:	0f b6 c0             	movzbl %al,%eax
 804914b:	c9                   	leave  
 804914c:	c3                   	ret    

0804914d <memcmp>:
 804914d:	55                   	push   %ebp
 804914e:	89 e5                	mov    %esp,%ebp
 8049150:	57                   	push   %edi
 8049151:	56                   	push   %esi
 8049152:	53                   	push   %ebx
 8049153:	83 ec 1c             	sub    $0x1c,%esp
 8049156:	8b 7d 08             	mov    0x8(%ebp),%edi
 8049159:	8b 5d 10             	mov    0x10(%ebp),%ebx
 804915c:	89 f9                	mov    %edi,%ecx
 804915e:	85 db                	test   %ebx,%ebx
 8049160:	0f 94 c0             	sete   %al
 8049163:	85 ff                	test   %edi,%edi
 8049165:	75 30                	jne    8049197 <memcmp+0x4a>
 8049167:	84 c0                	test   %al,%al
 8049169:	75 2c                	jne    8049197 <memcmp+0x4a>
 804916b:	c7 44 24 10 32 a0 04 	movl   $0x804a032,0x10(%esp)
 8049172:	08 
 8049173:	c7 44 24 0c 9f 9f 04 	movl   $0x8049f9f,0xc(%esp)
 804917a:	08 
 804917b:	c7 44 24 08 de 9e 04 	movl   $0x8049ede,0x8(%esp)
 8049182:	08 
 8049183:	c7 44 24 04 3a 00 00 	movl   $0x3a,0x4(%esp)
 804918a:	00 
 804918b:	c7 04 24 b6 9f 04 08 	movl   $0x8049fb6,(%esp)
 8049192:	e8 25 07 00 00       	call   80498bc <debug_panic>
 8049197:	8b 55 0c             	mov    0xc(%ebp),%edx
 804919a:	84 c0                	test   %al,%al
 804919c:	75 04                	jne    80491a2 <memcmp+0x55>
 804919e:	85 d2                	test   %edx,%edx
 80491a0:	74 1f                	je     80491c1 <memcmp+0x74>
 80491a2:	83 eb 01             	sub    $0x1,%ebx
 80491a5:	83 fb ff             	cmp    $0xffffffff,%ebx
 80491a8:	74 72                	je     804921c <memcmp+0xcf>
 80491aa:	0f b6 31             	movzbl (%ecx),%esi
 80491ad:	0f b6 12             	movzbl (%edx),%edx
 80491b0:	88 55 f3             	mov    %dl,-0xd(%ebp)
 80491b3:	89 fa                	mov    %edi,%edx
 80491b5:	8b 45 0c             	mov    0xc(%ebp),%eax
 80491b8:	89 f1                	mov    %esi,%ecx
 80491ba:	3a 4d f3             	cmp    -0xd(%ebp),%cl
 80491bd:	74 55                	je     8049214 <memcmp+0xc7>
 80491bf:	eb 44                	jmp    8049205 <memcmp+0xb8>
 80491c1:	c7 44 24 10 49 a0 04 	movl   $0x804a049,0x10(%esp)
 80491c8:	08 
 80491c9:	c7 44 24 0c 9f 9f 04 	movl   $0x8049f9f,0xc(%esp)
 80491d0:	08 
 80491d1:	c7 44 24 08 de 9e 04 	movl   $0x8049ede,0x8(%esp)
 80491d8:	08 
 80491d9:	c7 44 24 04 3b 00 00 	movl   $0x3b,0x4(%esp)
 80491e0:	00 
 80491e1:	c7 04 24 b6 9f 04 08 	movl   $0x8049fb6,(%esp)
 80491e8:	e8 cf 06 00 00       	call   80498bc <debug_panic>
 80491ed:	0f b6 72 01          	movzbl 0x1(%edx),%esi
 80491f1:	0f b6 48 01          	movzbl 0x1(%eax),%ecx
 80491f5:	88 4d f3             	mov    %cl,-0xd(%ebp)
 80491f8:	83 c2 01             	add    $0x1,%edx
 80491fb:	83 c0 01             	add    $0x1,%eax
 80491fe:	89 f1                	mov    %esi,%ecx
 8049200:	3a 4d f3             	cmp    -0xd(%ebp),%cl
 8049203:	74 0f                	je     8049214 <memcmp+0xc7>
 8049205:	89 f0                	mov    %esi,%eax
 8049207:	38 45 f3             	cmp    %al,-0xd(%ebp)
 804920a:	19 c0                	sbb    %eax,%eax
 804920c:	83 e0 02             	and    $0x2,%eax
 804920f:	83 e8 01             	sub    $0x1,%eax
 8049212:	eb 0d                	jmp    8049221 <memcmp+0xd4>
 8049214:	83 eb 01             	sub    $0x1,%ebx
 8049217:	83 fb ff             	cmp    $0xffffffff,%ebx
 804921a:	75 d1                	jne    80491ed <memcmp+0xa0>
 804921c:	b8 00 00 00 00       	mov    $0x0,%eax
 8049221:	83 c4 1c             	add    $0x1c,%esp
 8049224:	5b                   	pop    %ebx
 8049225:	5e                   	pop    %esi
 8049226:	5f                   	pop    %edi
 8049227:	5d                   	pop    %ebp
 8049228:	c3                   	ret    

08049229 <strstr>:
 8049229:	55                   	push   %ebp
 804922a:	89 e5                	mov    %esp,%ebp
 804922c:	57                   	push   %edi
 804922d:	56                   	push   %esi
 804922e:	53                   	push   %ebx
 804922f:	83 ec 0c             	sub    $0xc,%esp
 8049232:	be ff ff ff ff       	mov    $0xffffffff,%esi
 8049237:	8b 7d 08             	mov    0x8(%ebp),%edi
 804923a:	b8 00 00 00 00       	mov    $0x0,%eax
 804923f:	fc                   	cld    
 8049240:	89 f1                	mov    %esi,%ecx
 8049242:	f2 ae                	repnz scas %es:(%edi),%al
 8049244:	f7 d1                	not    %ecx
 8049246:	8d 59 ff             	lea    -0x1(%ecx),%ebx
 8049249:	8b 7d 0c             	mov    0xc(%ebp),%edi
 804924c:	89 f1                	mov    %esi,%ecx
 804924e:	f2 ae                	repnz scas %es:(%edi),%al
 8049250:	f7 d1                	not    %ecx
 8049252:	8d 51 ff             	lea    -0x1(%ecx),%edx
 8049255:	39 d3                	cmp    %edx,%ebx
 8049257:	73 39                	jae    8049292 <strstr+0x69>
 8049259:	eb 2d                	jmp    8049288 <strstr+0x5f>
 804925b:	8b 45 08             	mov    0x8(%ebp),%eax
 804925e:	01 d8                	add    %ebx,%eax
 8049260:	89 45 ec             	mov    %eax,-0x14(%ebp)
 8049263:	89 c6                	mov    %eax,%esi
 8049265:	8b 7d 0c             	mov    0xc(%ebp),%edi
 8049268:	89 d1                	mov    %edx,%ecx
 804926a:	fc                   	cld    
 804926b:	39 d2                	cmp    %edx,%edx
 804926d:	f3 a6                	repz cmpsb %es:(%edi),%ds:(%esi)
 804926f:	0f 97 45 eb          	seta   -0x15(%ebp)
 8049273:	0f 92 c0             	setb   %al
 8049276:	38 45 eb             	cmp    %al,-0x15(%ebp)
 8049279:	75 05                	jne    8049280 <strstr+0x57>
 804927b:	8b 45 ec             	mov    -0x14(%ebp),%eax
 804927e:	eb 1e                	jmp    804929e <strstr+0x75>
 8049280:	83 c3 01             	add    $0x1,%ebx
 8049283:	39 5d f0             	cmp    %ebx,-0x10(%ebp)
 8049286:	73 d3                	jae    804925b <strstr+0x32>
 8049288:	b8 00 00 00 00       	mov    $0x0,%eax
 804928d:	8d 76 00             	lea    0x0(%esi),%esi
 8049290:	eb 0c                	jmp    804929e <strstr+0x75>
 8049292:	29 d3                	sub    %edx,%ebx
 8049294:	89 5d f0             	mov    %ebx,-0x10(%ebp)
 8049297:	bb 00 00 00 00       	mov    $0x0,%ebx
 804929c:	eb bd                	jmp    804925b <strstr+0x32>
 804929e:	83 c4 0c             	add    $0xc,%esp
 80492a1:	5b                   	pop    %ebx
 80492a2:	5e                   	pop    %esi
 80492a3:	5f                   	pop    %edi
 80492a4:	5d                   	pop    %ebp
 80492a5:	c3                   	ret    

080492a6 <memmove>:
 80492a6:	55                   	push   %ebp
 80492a7:	89 e5                	mov    %esp,%ebp
 80492a9:	57                   	push   %edi
 80492aa:	56                   	push   %esi
 80492ab:	53                   	push   %ebx
 80492ac:	83 ec 1c             	sub    $0x1c,%esp
 80492af:	8b 5d 08             	mov    0x8(%ebp),%ebx
 80492b2:	8b 4d 0c             	mov    0xc(%ebp),%ecx
 80492b5:	8b 7d 10             	mov    0x10(%ebp),%edi
 80492b8:	89 de                	mov    %ebx,%esi
 80492ba:	85 ff                	test   %edi,%edi
 80492bc:	0f 94 c2             	sete   %dl
 80492bf:	85 db                	test   %ebx,%ebx
 80492c1:	75 30                	jne    80492f3 <memmove+0x4d>
 80492c3:	84 d2                	test   %dl,%dl
 80492c5:	75 2c                	jne    80492f3 <memmove+0x4d>
 80492c7:	c7 44 24 10 c6 9f 04 	movl   $0x8049fc6,0x10(%esp)
 80492ce:	08 
 80492cf:	c7 44 24 0c 9f 9f 04 	movl   $0x8049f9f,0xc(%esp)
 80492d6:	08 
 80492d7:	c7 44 24 08 e5 9e 04 	movl   $0x8049ee5,0x8(%esp)
 80492de:	08 
 80492df:	c7 44 24 04 1d 00 00 	movl   $0x1d,0x4(%esp)
 80492e6:	00 
 80492e7:	c7 04 24 b6 9f 04 08 	movl   $0x8049fb6,(%esp)
 80492ee:	e8 c9 05 00 00       	call   80498bc <debug_panic>
 80492f3:	89 c8                	mov    %ecx,%eax
 80492f5:	84 d2                	test   %dl,%dl
 80492f7:	75 30                	jne    8049329 <memmove+0x83>
 80492f9:	85 c9                	test   %ecx,%ecx
 80492fb:	75 2c                	jne    8049329 <memmove+0x83>
 80492fd:	c7 44 24 10 60 a0 04 	movl   $0x804a060,0x10(%esp)
 8049304:	08 
 8049305:	c7 44 24 0c 9f 9f 04 	movl   $0x8049f9f,0xc(%esp)
 804930c:	08 
 804930d:	c7 44 24 08 e5 9e 04 	movl   $0x8049ee5,0x8(%esp)
 8049314:	08 
 8049315:	c7 44 24 04 1e 00 00 	movl   $0x1e,0x4(%esp)
 804931c:	00 
 804931d:	c7 04 24 b6 9f 04 08 	movl   $0x8049fb6,(%esp)
 8049324:	e8 93 05 00 00       	call   80498bc <debug_panic>
 8049329:	39 c6                	cmp    %eax,%esi
 804932b:	73 1b                	jae    8049348 <memmove+0xa2>
 804932d:	85 ff                	test   %edi,%edi
 804932f:	74 39                	je     804936a <memmove+0xc4>
 8049331:	ba 00 00 00 00       	mov    $0x0,%edx
 8049336:	0f b6 04 0a          	movzbl (%edx,%ecx,1),%eax
 804933a:	88 04 1a             	mov    %al,(%edx,%ebx,1)
 804933d:	83 c2 01             	add    $0x1,%edx
 8049340:	39 fa                	cmp    %edi,%edx
 8049342:	75 f2                	jne    8049336 <memmove+0x90>
 8049344:	01 fe                	add    %edi,%esi
 8049346:	eb 22                	jmp    804936a <memmove+0xc4>
 8049348:	01 fe                	add    %edi,%esi
 804934a:	85 ff                	test   %edi,%edi
 804934c:	74 1c                	je     804936a <memmove+0xc4>
 804934e:	8d 0c 38             	lea    (%eax,%edi,1),%ecx
 8049351:	89 f2                	mov    %esi,%edx
 8049353:	89 f3                	mov    %esi,%ebx
 8049355:	29 fb                	sub    %edi,%ebx
 8049357:	0f b6 41 ff          	movzbl -0x1(%ecx),%eax
 804935b:	88 42 ff             	mov    %al,-0x1(%edx)
 804935e:	83 ea 01             	sub    $0x1,%edx
 8049361:	83 e9 01             	sub    $0x1,%ecx
 8049364:	39 da                	cmp    %ebx,%edx
 8049366:	75 ef                	jne    8049357 <memmove+0xb1>
 8049368:	29 fe                	sub    %edi,%esi
 804936a:	89 f0                	mov    %esi,%eax
 804936c:	83 c4 1c             	add    $0x1c,%esp
 804936f:	5b                   	pop    %ebx
 8049370:	5e                   	pop    %esi
 8049371:	5f                   	pop    %edi
 8049372:	5d                   	pop    %ebp
 8049373:	c3                   	ret    

08049374 <memcpy>:
 8049374:	55                   	push   %ebp
 8049375:	89 e5                	mov    %esp,%ebp
 8049377:	56                   	push   %esi
 8049378:	53                   	push   %ebx
 8049379:	83 ec 20             	sub    $0x20,%esp
 804937c:	8b 5d 08             	mov    0x8(%ebp),%ebx
 804937f:	8b 75 0c             	mov    0xc(%ebp),%esi
 8049382:	8b 4d 10             	mov    0x10(%ebp),%ecx
 8049385:	85 c9                	test   %ecx,%ecx
 8049387:	0f 94 c0             	sete   %al
 804938a:	85 db                	test   %ebx,%ebx
 804938c:	75 30                	jne    80493be <memcpy+0x4a>
 804938e:	84 c0                	test   %al,%al
 8049390:	75 2c                	jne    80493be <memcpy+0x4a>
 8049392:	c7 44 24 10 c6 9f 04 	movl   $0x8049fc6,0x10(%esp)
 8049399:	08 
 804939a:	c7 44 24 0c 9f 9f 04 	movl   $0x8049f9f,0xc(%esp)
 80493a1:	08 
 80493a2:	c7 44 24 08 ed 9e 04 	movl   $0x8049eed,0x8(%esp)
 80493a9:	08 
 80493aa:	c7 44 24 04 0c 00 00 	movl   $0xc,0x4(%esp)
 80493b1:	00 
 80493b2:	c7 04 24 b6 9f 04 08 	movl   $0x8049fb6,(%esp)
 80493b9:	e8 fe 04 00 00       	call   80498bc <debug_panic>
 80493be:	84 c0                	test   %al,%al
 80493c0:	75 04                	jne    80493c6 <memcpy+0x52>
 80493c2:	85 f6                	test   %esi,%esi
 80493c4:	74 0e                	je     80493d4 <memcpy+0x60>
 80493c6:	ba 00 00 00 00       	mov    $0x0,%edx
 80493cb:	85 c9                	test   %ecx,%ecx
 80493cd:	8d 76 00             	lea    0x0(%esi),%esi
 80493d0:	75 2e                	jne    8049400 <memcpy+0x8c>
 80493d2:	eb 3a                	jmp    804940e <memcpy+0x9a>
 80493d4:	c7 44 24 10 60 a0 04 	movl   $0x804a060,0x10(%esp)
 80493db:	08 
 80493dc:	c7 44 24 0c 9f 9f 04 	movl   $0x8049f9f,0xc(%esp)
 80493e3:	08 
 80493e4:	c7 44 24 08 ed 9e 04 	movl   $0x8049eed,0x8(%esp)
 80493eb:	08 
 80493ec:	c7 44 24 04 0d 00 00 	movl   $0xd,0x4(%esp)
 80493f3:	00 
 80493f4:	c7 04 24 b6 9f 04 08 	movl   $0x8049fb6,(%esp)
 80493fb:	e8 bc 04 00 00       	call   80498bc <debug_panic>
 8049400:	0f b6 04 32          	movzbl (%edx,%esi,1),%eax
 8049404:	88 04 1a             	mov    %al,(%edx,%ebx,1)
 8049407:	83 c2 01             	add    $0x1,%edx
 804940a:	39 ca                	cmp    %ecx,%edx
 804940c:	75 f2                	jne    8049400 <memcpy+0x8c>
 804940e:	89 d8                	mov    %ebx,%eax
 8049410:	83 c4 20             	add    $0x20,%esp
 8049413:	5b                   	pop    %ebx
 8049414:	5e                   	pop    %esi
 8049415:	5d                   	pop    %ebp
 8049416:	c3                   	ret    

08049417 <strlcat>:
 8049417:	55                   	push   %ebp
 8049418:	89 e5                	mov    %esp,%ebp
 804941a:	83 ec 28             	sub    $0x28,%esp
 804941d:	89 5d f4             	mov    %ebx,-0xc(%ebp)
 8049420:	89 75 f8             	mov    %esi,-0x8(%ebp)
 8049423:	89 7d fc             	mov    %edi,-0x4(%ebp)
 8049426:	8b 5d 10             	mov    0x10(%ebp),%ebx
 8049429:	83 7d 08 00          	cmpl   $0x0,0x8(%ebp)
 804942d:	75 2c                	jne    804945b <strlcat+0x44>
 804942f:	c7 44 24 10 79 a0 04 	movl   $0x804a079,0x10(%esp)
 8049436:	08 
 8049437:	c7 44 24 0c 9f 9f 04 	movl   $0x8049f9f,0xc(%esp)
 804943e:	08 
 804943f:	c7 44 24 08 a2 9e 04 	movl   $0x8049ea2,0x8(%esp)
 8049446:	08 
 8049447:	c7 44 24 04 68 01 00 	movl   $0x168,0x4(%esp)
 804944e:	00 
 804944f:	c7 04 24 b6 9f 04 08 	movl   $0x8049fb6,(%esp)
 8049456:	e8 61 04 00 00       	call   80498bc <debug_panic>
 804945b:	83 7d 0c 00          	cmpl   $0x0,0xc(%ebp)
 804945f:	75 2c                	jne    804948d <strlcat+0x76>
 8049461:	c7 44 24 10 85 a0 04 	movl   $0x804a085,0x10(%esp)
 8049468:	08 
 8049469:	c7 44 24 0c 9f 9f 04 	movl   $0x8049f9f,0xc(%esp)
 8049470:	08 
 8049471:	c7 44 24 08 a2 9e 04 	movl   $0x8049ea2,0x8(%esp)
 8049478:	08 
 8049479:	c7 44 24 04 69 01 00 	movl   $0x169,0x4(%esp)
 8049480:	00 
 8049481:	c7 04 24 b6 9f 04 08 	movl   $0x8049fb6,(%esp)
 8049488:	e8 2f 04 00 00       	call   80498bc <debug_panic>
 804948d:	ba ff ff ff ff       	mov    $0xffffffff,%edx
 8049492:	8b 7d 0c             	mov    0xc(%ebp),%edi
 8049495:	b8 00 00 00 00       	mov    $0x0,%eax
 804949a:	fc                   	cld    
 804949b:	89 d1                	mov    %edx,%ecx
 804949d:	f2 ae                	repnz scas %es:(%edi),%al
 804949f:	f7 d1                	not    %ecx
 80494a1:	8d 71 ff             	lea    -0x1(%ecx),%esi
 80494a4:	8b 7d 08             	mov    0x8(%ebp),%edi
 80494a7:	89 d1                	mov    %edx,%ecx
 80494a9:	f2 ae                	repnz scas %es:(%edi),%al
 80494ab:	f7 d1                	not    %ecx
 80494ad:	8d 79 ff             	lea    -0x1(%ecx),%edi
 80494b0:	85 db                	test   %ebx,%ebx
 80494b2:	74 33                	je     80494e7 <strlcat+0xd0>
 80494b4:	39 df                	cmp    %ebx,%edi
 80494b6:	73 2f                	jae    80494e7 <strlcat+0xd0>
 80494b8:	89 d8                	mov    %ebx,%eax
 80494ba:	29 f8                	sub    %edi,%eax
 80494bc:	8d 58 ff             	lea    -0x1(%eax),%ebx
 80494bf:	39 f3                	cmp    %esi,%ebx
 80494c1:	76 02                	jbe    80494c5 <strlcat+0xae>
 80494c3:	89 f3                	mov    %esi,%ebx
 80494c5:	8b 45 08             	mov    0x8(%ebp),%eax
 80494c8:	01 f8                	add    %edi,%eax
 80494ca:	89 5c 24 08          	mov    %ebx,0x8(%esp)
 80494ce:	8b 55 0c             	mov    0xc(%ebp),%edx
 80494d1:	89 54 24 04          	mov    %edx,0x4(%esp)
 80494d5:	89 04 24             	mov    %eax,(%esp)
 80494d8:	e8 97 fe ff ff       	call   8049374 <memcpy>
 80494dd:	8d 04 3b             	lea    (%ebx,%edi,1),%eax
 80494e0:	8b 55 08             	mov    0x8(%ebp),%edx
 80494e3:	c6 04 10 00          	movb   $0x0,(%eax,%edx,1)
 80494e7:	8d 04 37             	lea    (%edi,%esi,1),%eax
 80494ea:	8b 5d f4             	mov    -0xc(%ebp),%ebx
 80494ed:	8b 75 f8             	mov    -0x8(%ebp),%esi
 80494f0:	8b 7d fc             	mov    -0x4(%ebp),%edi
 80494f3:	89 ec                	mov    %ebp,%esp
 80494f5:	5d                   	pop    %ebp
 80494f6:	c3                   	ret    

080494f7 <strlcpy>:
 80494f7:	55                   	push   %ebp
 80494f8:	89 e5                	mov    %esp,%ebp
 80494fa:	83 ec 28             	sub    $0x28,%esp
 80494fd:	89 5d f4             	mov    %ebx,-0xc(%ebp)
 8049500:	89 75 f8             	mov    %esi,-0x8(%ebp)
 8049503:	89 7d fc             	mov    %edi,-0x4(%ebp)
 8049506:	8b 75 08             	mov    0x8(%ebp),%esi
 8049509:	8b 55 0c             	mov    0xc(%ebp),%edx
 804950c:	85 f6                	test   %esi,%esi
 804950e:	75 2c                	jne    804953c <strlcpy+0x45>
 8049510:	c7 44 24 10 79 a0 04 	movl   $0x804a079,0x10(%esp)
 8049517:	08 
 8049518:	c7 44 24 0c 9f 9f 04 	movl   $0x8049f9f,0xc(%esp)
 804951f:	08 
 8049520:	c7 44 24 08 aa 9e 04 	movl   $0x8049eaa,0x8(%esp)
 8049527:	08 
 8049528:	c7 44 24 04 4a 01 00 	movl   $0x14a,0x4(%esp)
 804952f:	00 
 8049530:	c7 04 24 b6 9f 04 08 	movl   $0x8049fb6,(%esp)
 8049537:	e8 80 03 00 00       	call   80498bc <debug_panic>
 804953c:	85 d2                	test   %edx,%edx
 804953e:	75 2c                	jne    804956c <strlcpy+0x75>
 8049540:	c7 44 24 10 85 a0 04 	movl   $0x804a085,0x10(%esp)
 8049547:	08 
 8049548:	c7 44 24 0c 9f 9f 04 	movl   $0x8049f9f,0xc(%esp)
 804954f:	08 
 8049550:	c7 44 24 08 aa 9e 04 	movl   $0x8049eaa,0x8(%esp)
 8049557:	08 
 8049558:	c7 44 24 04 4b 01 00 	movl   $0x14b,0x4(%esp)
 804955f:	00 
 8049560:	c7 04 24 b6 9f 04 08 	movl   $0x8049fb6,(%esp)
 8049567:	e8 50 03 00 00       	call   80498bc <debug_panic>
 804956c:	89 d7                	mov    %edx,%edi
 804956e:	fc                   	cld    
 804956f:	b9 ff ff ff ff       	mov    $0xffffffff,%ecx
 8049574:	b8 00 00 00 00       	mov    $0x0,%eax
 8049579:	f2 ae                	repnz scas %es:(%edi),%al
 804957b:	f7 d1                	not    %ecx
 804957d:	8d 59 ff             	lea    -0x1(%ecx),%ebx
 8049580:	83 7d 10 00          	cmpl   $0x0,0x10(%ebp)
 8049584:	74 20                	je     80495a6 <strlcpy+0xaf>
 8049586:	8b 7d 10             	mov    0x10(%ebp),%edi
 8049589:	83 ef 01             	sub    $0x1,%edi
 804958c:	39 df                	cmp    %ebx,%edi
 804958e:	76 02                	jbe    8049592 <strlcpy+0x9b>
 8049590:	89 df                	mov    %ebx,%edi
 8049592:	89 7c 24 08          	mov    %edi,0x8(%esp)
 8049596:	89 54 24 04          	mov    %edx,0x4(%esp)
 804959a:	89 34 24             	mov    %esi,(%esp)
 804959d:	e8 d2 fd ff ff       	call   8049374 <memcpy>
 80495a2:	c6 04 3e 00          	movb   $0x0,(%esi,%edi,1)
 80495a6:	89 d8                	mov    %ebx,%eax
 80495a8:	8b 5d f4             	mov    -0xc(%ebp),%ebx
 80495ab:	8b 75 f8             	mov    -0x8(%ebp),%esi
 80495ae:	8b 7d fc             	mov    -0x4(%ebp),%edi
 80495b1:	89 ec                	mov    %ebp,%esp
 80495b3:	5d                   	pop    %ebp
 80495b4:	c3                   	ret    
 80495b5:	90                   	nop    
 80495b6:	90                   	nop    
 80495b7:	90                   	nop    
 80495b8:	90                   	nop    
 80495b9:	90                   	nop    
 80495ba:	90                   	nop    
 80495bb:	90                   	nop    
 80495bc:	90                   	nop    
 80495bd:	90                   	nop    
 80495be:	90                   	nop    
 80495bf:	90                   	nop    

080495c0 <udiv64>:
 80495c0:	55                   	push   %ebp
 80495c1:	89 e5                	mov    %esp,%ebp
 80495c3:	83 ec 58             	sub    $0x58,%esp
 80495c6:	89 5d f4             	mov    %ebx,-0xc(%ebp)
 80495c9:	89 75 f8             	mov    %esi,-0x8(%ebp)
 80495cc:	89 7d fc             	mov    %edi,-0x4(%ebp)
 80495cf:	89 45 d8             	mov    %eax,-0x28(%ebp)
 80495d2:	89 55 dc             	mov    %edx,-0x24(%ebp)
 80495d5:	8b 45 08             	mov    0x8(%ebp),%eax
 80495d8:	8b 55 0c             	mov    0xc(%ebp),%edx
 80495db:	89 45 d0             	mov    %eax,-0x30(%ebp)
 80495de:	89 55 d4             	mov    %edx,-0x2c(%ebp)
 80495e1:	89 d0                	mov    %edx,%eax
 80495e3:	31 d2                	xor    %edx,%edx
 80495e5:	89 d1                	mov    %edx,%ecx
 80495e7:	09 c1                	or     %eax,%ecx
 80495e9:	75 4c                	jne    8049637 <udiv64+0x77>
 80495eb:	8b 5d dc             	mov    -0x24(%ebp),%ebx
 80495ee:	8b 75 d0             	mov    -0x30(%ebp),%esi
 80495f1:	89 d8                	mov    %ebx,%eax
 80495f3:	ba 00 00 00 00       	mov    $0x0,%edx
 80495f8:	f7 f6                	div    %esi
 80495fa:	89 c7                	mov    %eax,%edi
 80495fc:	89 d3                	mov    %edx,%ebx
 80495fe:	b9 00 00 00 00       	mov    $0x0,%ecx
 8049603:	8b 45 d8             	mov    -0x28(%ebp),%eax
 8049606:	ba 00 00 00 00       	mov    $0x0,%edx
 804960b:	01 c1                	add    %eax,%ecx
 804960d:	11 d3                	adc    %edx,%ebx
 804960f:	31 d2                	xor    %edx,%edx
 8049611:	89 5d c8             	mov    %ebx,-0x38(%ebp)
 8049614:	89 55 cc             	mov    %edx,-0x34(%ebp)
 8049617:	8b 55 c8             	mov    -0x38(%ebp),%edx
 804961a:	89 c8                	mov    %ecx,%eax
 804961c:	f7 f6                	div    %esi
 804961e:	89 c1                	mov    %eax,%ecx
 8049620:	bb 00 00 00 00       	mov    $0x0,%ebx
 8049625:	b8 00 00 00 00       	mov    $0x0,%eax
 804962a:	89 de                	mov    %ebx,%esi
 804962c:	89 cb                	mov    %ecx,%ebx
 804962e:	01 c3                	add    %eax,%ebx
 8049630:	11 fe                	adc    %edi,%esi
 8049632:	e9 0d 01 00 00       	jmp    8049744 <udiv64+0x184>
 8049637:	8b 4d dc             	mov    -0x24(%ebp),%ecx
 804963a:	39 4d d4             	cmp    %ecx,-0x2c(%ebp)
 804963d:	72 19                	jb     8049658 <udiv64+0x98>
 804963f:	77 08                	ja     8049649 <udiv64+0x89>
 8049641:	8b 4d d8             	mov    -0x28(%ebp),%ecx
 8049644:	39 4d d0             	cmp    %ecx,-0x30(%ebp)
 8049647:	76 0f                	jbe    8049658 <udiv64+0x98>
 8049649:	bb 00 00 00 00       	mov    $0x0,%ebx
 804964e:	be 00 00 00 00       	mov    $0x0,%esi
 8049653:	e9 ec 00 00 00       	jmp    8049744 <udiv64+0x184>
 8049658:	c7 45 e4 00 00 00 00 	movl   $0x0,-0x1c(%ebp)
 804965f:	3d ff ff 00 00       	cmp    $0xffff,%eax
 8049664:	77 0a                	ja     8049670 <udiv64+0xb0>
 8049666:	c1 e0 10             	shl    $0x10,%eax
 8049669:	c7 45 e4 10 00 00 00 	movl   $0x10,-0x1c(%ebp)
 8049670:	3d ff ff ff 00       	cmp    $0xffffff,%eax
 8049675:	77 07                	ja     804967e <udiv64+0xbe>
 8049677:	83 45 e4 08          	addl   $0x8,-0x1c(%ebp)
 804967b:	c1 e0 08             	shl    $0x8,%eax
 804967e:	3d ff ff ff 0f       	cmp    $0xfffffff,%eax
 8049683:	77 07                	ja     804968c <udiv64+0xcc>
 8049685:	83 45 e4 04          	addl   $0x4,-0x1c(%ebp)
 8049689:	c1 e0 04             	shl    $0x4,%eax
 804968c:	3d ff ff ff 3f       	cmp    $0x3fffffff,%eax
 8049691:	77 07                	ja     804969a <udiv64+0xda>
 8049693:	83 45 e4 02          	addl   $0x2,-0x1c(%ebp)
 8049697:	c1 e0 02             	shl    $0x2,%eax
 804969a:	3d 00 00 00 80       	cmp    $0x80000000,%eax
 804969f:	83 55 e4 00          	adcl   $0x0,-0x1c(%ebp)
 80496a3:	8b 75 d8             	mov    -0x28(%ebp),%esi
 80496a6:	8b 7d dc             	mov    -0x24(%ebp),%edi
 80496a9:	0f ac fe 01          	shrd   $0x1,%edi,%esi
 80496ad:	d1 ef                	shr    %edi
 80496af:	31 d2                	xor    %edx,%edx
 80496b1:	89 7d c0             	mov    %edi,-0x40(%ebp)
 80496b4:	89 55 c4             	mov    %edx,-0x3c(%ebp)
 80496b7:	8b 45 d0             	mov    -0x30(%ebp),%eax
 80496ba:	8b 55 d4             	mov    -0x2c(%ebp),%edx
 80496bd:	0f b6 4d e4          	movzbl -0x1c(%ebp),%ecx
 80496c1:	0f a5 c2             	shld   %cl,%eax,%edx
 80496c4:	d3 e0                	shl    %cl,%eax
 80496c6:	f6 c1 20             	test   $0x20,%cl
 80496c9:	74 02                	je     80496cd <udiv64+0x10d>
 80496cb:	89 c2                	mov    %eax,%edx
 80496cd:	89 d0                	mov    %edx,%eax
 80496cf:	31 d2                	xor    %edx,%edx
 80496d1:	89 45 b8             	mov    %eax,-0x48(%ebp)
 80496d4:	89 55 bc             	mov    %edx,-0x44(%ebp)
 80496d7:	8b 55 c0             	mov    -0x40(%ebp),%edx
 80496da:	89 f0                	mov    %esi,%eax
 80496dc:	f7 75 b8             	divl   -0x48(%ebp)
 80496df:	b9 1f 00 00 00       	mov    $0x1f,%ecx
 80496e4:	2b 4d e4             	sub    -0x1c(%ebp),%ecx
 80496e7:	d3 e8                	shr    %cl,%eax
 80496e9:	89 45 e8             	mov    %eax,-0x18(%ebp)
 80496ec:	c7 45 ec 00 00 00 00 	movl   $0x0,-0x14(%ebp)
 80496f3:	8b 5d e8             	mov    -0x18(%ebp),%ebx
 80496f6:	8b 75 ec             	mov    -0x14(%ebp),%esi
 80496f9:	83 c3 ff             	add    $0xffffffff,%ebx
 80496fc:	83 d6 ff             	adc    $0xffffffff,%esi
 80496ff:	8b 4d d0             	mov    -0x30(%ebp),%ecx
 8049702:	0f af ce             	imul   %esi,%ecx
 8049705:	8b 45 d4             	mov    -0x2c(%ebp),%eax
 8049708:	0f af c3             	imul   %ebx,%eax
 804970b:	01 c1                	add    %eax,%ecx
 804970d:	8b 45 d0             	mov    -0x30(%ebp),%eax
 8049710:	f7 e3                	mul    %ebx
 8049712:	89 45 b0             	mov    %eax,-0x50(%ebp)
 8049715:	01 ca                	add    %ecx,%edx
 8049717:	89 55 b4             	mov    %edx,-0x4c(%ebp)
 804971a:	8b 55 d8             	mov    -0x28(%ebp),%edx
 804971d:	8b 4d dc             	mov    -0x24(%ebp),%ecx
 8049720:	2b 55 b0             	sub    -0x50(%ebp),%edx
 8049723:	1b 4d b4             	sbb    -0x4c(%ebp),%ecx
 8049726:	89 55 a8             	mov    %edx,-0x58(%ebp)
 8049729:	89 4d ac             	mov    %ecx,-0x54(%ebp)
 804972c:	39 4d d4             	cmp    %ecx,-0x2c(%ebp)
 804972f:	77 13                	ja     8049744 <udiv64+0x184>
 8049731:	8b 4d ac             	mov    -0x54(%ebp),%ecx
 8049734:	39 4d d4             	cmp    %ecx,-0x2c(%ebp)
 8049737:	72 05                	jb     804973e <udiv64+0x17e>
 8049739:	39 55 d0             	cmp    %edx,-0x30(%ebp)
 804973c:	77 06                	ja     8049744 <udiv64+0x184>
 804973e:	8b 5d e8             	mov    -0x18(%ebp),%ebx
 8049741:	8b 75 ec             	mov    -0x14(%ebp),%esi
 8049744:	89 d8                	mov    %ebx,%eax
 8049746:	89 f2                	mov    %esi,%edx
 8049748:	8b 5d f4             	mov    -0xc(%ebp),%ebx
 804974b:	8b 75 f8             	mov    -0x8(%ebp),%esi
 804974e:	8b 7d fc             	mov    -0x4(%ebp),%edi
 8049751:	89 ec                	mov    %ebp,%esp
 8049753:	5d                   	pop    %ebp
 8049754:	c3                   	ret    

08049755 <sdiv64>:
 8049755:	55                   	push   %ebp
 8049756:	89 e5                	mov    %esp,%ebp
 8049758:	56                   	push   %esi
 8049759:	53                   	push   %ebx
 804975a:	83 ec 20             	sub    $0x20,%esp
 804975d:	89 c3                	mov    %eax,%ebx
 804975f:	89 d6                	mov    %edx,%esi
 8049761:	8b 45 08             	mov    0x8(%ebp),%eax
 8049764:	8b 55 0c             	mov    0xc(%ebp),%edx
 8049767:	89 45 e0             	mov    %eax,-0x20(%ebp)
 804976a:	89 55 e4             	mov    %edx,-0x1c(%ebp)
 804976d:	89 5d e8             	mov    %ebx,-0x18(%ebp)
 8049770:	89 75 ec             	mov    %esi,-0x14(%ebp)
 8049773:	85 f6                	test   %esi,%esi
 8049775:	79 11                	jns    8049788 <sdiv64+0x33>
 8049777:	89 d8                	mov    %ebx,%eax
 8049779:	89 f2                	mov    %esi,%edx
 804977b:	f7 d8                	neg    %eax
 804977d:	83 d2 00             	adc    $0x0,%edx
 8049780:	f7 da                	neg    %edx
 8049782:	89 45 e8             	mov    %eax,-0x18(%ebp)
 8049785:	89 55 ec             	mov    %edx,-0x14(%ebp)
 8049788:	8b 45 e0             	mov    -0x20(%ebp),%eax
 804978b:	8b 55 e4             	mov    -0x1c(%ebp),%edx
 804978e:	83 7d e4 00          	cmpl   $0x0,-0x1c(%ebp)
 8049792:	79 0d                	jns    80497a1 <sdiv64+0x4c>
 8049794:	8b 45 e0             	mov    -0x20(%ebp),%eax
 8049797:	8b 55 e4             	mov    -0x1c(%ebp),%edx
 804979a:	f7 d8                	neg    %eax
 804979c:	83 d2 00             	adc    $0x0,%edx
 804979f:	f7 da                	neg    %edx
 80497a1:	89 04 24             	mov    %eax,(%esp)
 80497a4:	89 54 24 04          	mov    %edx,0x4(%esp)
 80497a8:	8b 45 e8             	mov    -0x18(%ebp),%eax
 80497ab:	8b 55 ec             	mov    -0x14(%ebp),%edx
 80497ae:	e8 0d fe ff ff       	call   80495c0 <udiv64>
 80497b3:	89 45 f0             	mov    %eax,-0x10(%ebp)
 80497b6:	89 55 f4             	mov    %edx,-0xc(%ebp)
 80497b9:	89 f0                	mov    %esi,%eax
 80497bb:	f7 d0                	not    %eax
 80497bd:	c1 e8 1f             	shr    $0x1f,%eax
 80497c0:	8b 55 e4             	mov    -0x1c(%ebp),%edx
 80497c3:	c1 ea 1f             	shr    $0x1f,%edx
 80497c6:	8b 4d f0             	mov    -0x10(%ebp),%ecx
 80497c9:	8b 5d f4             	mov    -0xc(%ebp),%ebx
 80497cc:	39 d0                	cmp    %edx,%eax
 80497ce:	75 0d                	jne    80497dd <sdiv64+0x88>
 80497d0:	8b 4d f0             	mov    -0x10(%ebp),%ecx
 80497d3:	8b 5d f4             	mov    -0xc(%ebp),%ebx
 80497d6:	f7 d9                	neg    %ecx
 80497d8:	83 d3 00             	adc    $0x0,%ebx
 80497db:	f7 db                	neg    %ebx
 80497dd:	89 c8                	mov    %ecx,%eax
 80497df:	89 da                	mov    %ebx,%edx
 80497e1:	83 c4 20             	add    $0x20,%esp
 80497e4:	5b                   	pop    %ebx
 80497e5:	5e                   	pop    %esi
 80497e6:	5d                   	pop    %ebp
 80497e7:	c3                   	ret    

080497e8 <__divdi3>:
 80497e8:	55                   	push   %ebp
 80497e9:	89 e5                	mov    %esp,%ebp
 80497eb:	53                   	push   %ebx
 80497ec:	83 ec 0c             	sub    $0xc,%esp
 80497ef:	8b 45 08             	mov    0x8(%ebp),%eax
 80497f2:	8b 55 0c             	mov    0xc(%ebp),%edx
 80497f5:	8b 4d 10             	mov    0x10(%ebp),%ecx
 80497f8:	8b 5d 14             	mov    0x14(%ebp),%ebx
 80497fb:	89 0c 24             	mov    %ecx,(%esp)
 80497fe:	89 5c 24 04          	mov    %ebx,0x4(%esp)
 8049802:	e8 4e ff ff ff       	call   8049755 <sdiv64>
 8049807:	83 c4 0c             	add    $0xc,%esp
 804980a:	5b                   	pop    %ebx
 804980b:	5d                   	pop    %ebp
 804980c:	c3                   	ret    

0804980d <__moddi3>:
 804980d:	55                   	push   %ebp
 804980e:	89 e5                	mov    %esp,%ebp
 8049810:	83 ec 18             	sub    $0x18,%esp
 8049813:	89 5d f4             	mov    %ebx,-0xc(%ebp)
 8049816:	89 75 f8             	mov    %esi,-0x8(%ebp)
 8049819:	89 7d fc             	mov    %edi,-0x4(%ebp)
 804981c:	8b 75 08             	mov    0x8(%ebp),%esi
 804981f:	8b 7d 0c             	mov    0xc(%ebp),%edi
 8049822:	8b 45 10             	mov    0x10(%ebp),%eax
 8049825:	8b 55 14             	mov    0x14(%ebp),%edx
 8049828:	89 04 24             	mov    %eax,(%esp)
 804982b:	89 54 24 04          	mov    %edx,0x4(%esp)
 804982f:	89 f0                	mov    %esi,%eax
 8049831:	89 fa                	mov    %edi,%edx
 8049833:	e8 1d ff ff ff       	call   8049755 <sdiv64>
 8049838:	f7 65 10             	mull   0x10(%ebp)
 804983b:	29 c6                	sub    %eax,%esi
 804983d:	89 f0                	mov    %esi,%eax
 804983f:	89 f2                	mov    %esi,%edx
 8049841:	c1 fa 1f             	sar    $0x1f,%edx
 8049844:	8b 5d f4             	mov    -0xc(%ebp),%ebx
 8049847:	8b 75 f8             	mov    -0x8(%ebp),%esi
 804984a:	8b 7d fc             	mov    -0x4(%ebp),%edi
 804984d:	89 ec                	mov    %ebp,%esp
 804984f:	5d                   	pop    %ebp
 8049850:	c3                   	ret    

08049851 <__udivdi3>:
 8049851:	55                   	push   %ebp
 8049852:	89 e5                	mov    %esp,%ebp
 8049854:	53                   	push   %ebx
 8049855:	83 ec 0c             	sub    $0xc,%esp
 8049858:	8b 45 08             	mov    0x8(%ebp),%eax
 804985b:	8b 55 0c             	mov    0xc(%ebp),%edx
 804985e:	8b 4d 10             	mov    0x10(%ebp),%ecx
 8049861:	8b 5d 14             	mov    0x14(%ebp),%ebx
 8049864:	89 0c 24             	mov    %ecx,(%esp)
 8049867:	89 5c 24 04          	mov    %ebx,0x4(%esp)
 804986b:	e8 50 fd ff ff       	call   80495c0 <udiv64>
 8049870:	83 c4 0c             	add    $0xc,%esp
 8049873:	5b                   	pop    %ebx
 8049874:	5d                   	pop    %ebp
 8049875:	c3                   	ret    

08049876 <__umoddi3>:
 8049876:	55                   	push   %ebp
 8049877:	89 e5                	mov    %esp,%ebp
 8049879:	83 ec 18             	sub    $0x18,%esp
 804987c:	89 5d f4             	mov    %ebx,-0xc(%ebp)
 804987f:	89 75 f8             	mov    %esi,-0x8(%ebp)
 8049882:	89 7d fc             	mov    %edi,-0x4(%ebp)
 8049885:	8b 75 08             	mov    0x8(%ebp),%esi
 8049888:	8b 7d 0c             	mov    0xc(%ebp),%edi
 804988b:	8b 45 10             	mov    0x10(%ebp),%eax
 804988e:	8b 55 14             	mov    0x14(%ebp),%edx
 8049891:	89 04 24             	mov    %eax,(%esp)
 8049894:	89 54 24 04          	mov    %edx,0x4(%esp)
 8049898:	89 f0                	mov    %esi,%eax
 804989a:	89 fa                	mov    %edi,%edx
 804989c:	e8 1f fd ff ff       	call   80495c0 <udiv64>
 80498a1:	f7 65 10             	mull   0x10(%ebp)
 80498a4:	29 c6                	sub    %eax,%esi
 80498a6:	89 f0                	mov    %esi,%eax
 80498a8:	ba 00 00 00 00       	mov    $0x0,%edx
 80498ad:	8b 5d f4             	mov    -0xc(%ebp),%ebx
 80498b0:	8b 75 f8             	mov    -0x8(%ebp),%esi
 80498b3:	8b 7d fc             	mov    -0x4(%ebp),%edi
 80498b6:	89 ec                	mov    %ebp,%esp
 80498b8:	5d                   	pop    %ebp
 80498b9:	c3                   	ret    
 80498ba:	90                   	nop    
 80498bb:	90                   	nop    

080498bc <debug_panic>:
 80498bc:	55                   	push   %ebp
 80498bd:	89 e5                	mov    %esp,%ebp
 80498bf:	83 ec 28             	sub    $0x28,%esp
 80498c2:	8b 45 10             	mov    0x10(%ebp),%eax
 80498c5:	89 44 24 0c          	mov    %eax,0xc(%esp)
 80498c9:	8b 45 0c             	mov    0xc(%ebp),%eax
 80498cc:	89 44 24 08          	mov    %eax,0x8(%esp)
 80498d0:	8b 45 08             	mov    0x8(%ebp),%eax
 80498d3:	89 44 24 04          	mov    %eax,0x4(%esp)
 80498d7:	c7 04 24 dc a0 04 08 	movl   $0x804a0dc,(%esp)
 80498de:	e8 39 f2 ff ff       	call   8048b1c <printf>
 80498e3:	8d 45 18             	lea    0x18(%ebp),%eax
 80498e6:	89 45 fc             	mov    %eax,-0x4(%ebp)
 80498e9:	89 44 24 04          	mov    %eax,0x4(%esp)
 80498ed:	8b 45 14             	mov    0x14(%ebp),%eax
 80498f0:	89 04 24             	mov    %eax,(%esp)
 80498f3:	e8 8d 02 00 00       	call   8049b85 <vprintf>
 80498f8:	c7 04 24 0a 00 00 00 	movl   $0xa,(%esp)
 80498ff:	e8 a3 02 00 00       	call   8049ba7 <putchar>
 8049904:	e8 3b 03 00 00       	call   8049c44 <debug_backtrace>
 8049909:	c7 04 24 01 00 00 00 	movl   $0x1,(%esp)
 8049910:	e8 59 01 00 00       	call   8049a6e <exit>
 8049915:	90                   	nop    
 8049916:	90                   	nop    
 8049917:	90                   	nop    

08049918 <exec>:
 8049918:	55                   	push   %ebp
 8049919:	89 e5                	mov    %esp,%ebp
 804991b:	ff 75 08             	pushl  0x8(%ebp)
 804991e:	6a 02                	push   $0x2
 8049920:	cd 30                	int    $0x30
 8049922:	83 c4 08             	add    $0x8,%esp
 8049925:	5d                   	pop    %ebp
 8049926:	c3                   	ret    

08049927 <wait>:
 8049927:	55                   	push   %ebp
 8049928:	89 e5                	mov    %esp,%ebp
 804992a:	ff 75 08             	pushl  0x8(%ebp)
 804992d:	6a 03                	push   $0x3
 804992f:	cd 30                	int    $0x30
 8049931:	83 c4 08             	add    $0x8,%esp
 8049934:	5d                   	pop    %ebp
 8049935:	c3                   	ret    

08049936 <create>:
 8049936:	55                   	push   %ebp
 8049937:	89 e5                	mov    %esp,%ebp
 8049939:	ff 75 0c             	pushl  0xc(%ebp)
 804993c:	ff 75 08             	pushl  0x8(%ebp)
 804993f:	6a 04                	push   $0x4
 8049941:	cd 30                	int    $0x30
 8049943:	83 c4 0c             	add    $0xc,%esp
 8049946:	85 c0                	test   %eax,%eax
 8049948:	0f 95 c0             	setne  %al
 804994b:	0f b6 c0             	movzbl %al,%eax
 804994e:	5d                   	pop    %ebp
 804994f:	c3                   	ret    

08049950 <remove>:
 8049950:	55                   	push   %ebp
 8049951:	89 e5                	mov    %esp,%ebp
 8049953:	ff 75 08             	pushl  0x8(%ebp)
 8049956:	6a 05                	push   $0x5
 8049958:	cd 30                	int    $0x30
 804995a:	83 c4 08             	add    $0x8,%esp
 804995d:	85 c0                	test   %eax,%eax
 804995f:	0f 95 c0             	setne  %al
 8049962:	0f b6 c0             	movzbl %al,%eax
 8049965:	5d                   	pop    %ebp
 8049966:	c3                   	ret    

08049967 <open>:
 8049967:	55                   	push   %ebp
 8049968:	89 e5                	mov    %esp,%ebp
 804996a:	ff 75 08             	pushl  0x8(%ebp)
 804996d:	6a 06                	push   $0x6
 804996f:	cd 30                	int    $0x30
 8049971:	83 c4 08             	add    $0x8,%esp
 8049974:	5d                   	pop    %ebp
 8049975:	c3                   	ret    

08049976 <filesize>:
 8049976:	55                   	push   %ebp
 8049977:	89 e5                	mov    %esp,%ebp
 8049979:	ff 75 08             	pushl  0x8(%ebp)
 804997c:	6a 07                	push   $0x7
 804997e:	cd 30                	int    $0x30
 8049980:	83 c4 08             	add    $0x8,%esp
 8049983:	5d                   	pop    %ebp
 8049984:	c3                   	ret    

08049985 <read>:
 8049985:	55                   	push   %ebp
 8049986:	89 e5                	mov    %esp,%ebp
 8049988:	ff 75 10             	pushl  0x10(%ebp)
 804998b:	ff 75 0c             	pushl  0xc(%ebp)
 804998e:	ff 75 08             	pushl  0x8(%ebp)
 8049991:	6a 08                	push   $0x8
 8049993:	cd 30                	int    $0x30
 8049995:	83 c4 10             	add    $0x10,%esp
 8049998:	5d                   	pop    %ebp
 8049999:	c3                   	ret    

0804999a <write>:
 804999a:	55                   	push   %ebp
 804999b:	89 e5                	mov    %esp,%ebp
 804999d:	ff 75 10             	pushl  0x10(%ebp)
 80499a0:	ff 75 0c             	pushl  0xc(%ebp)
 80499a3:	ff 75 08             	pushl  0x8(%ebp)
 80499a6:	6a 09                	push   $0x9
 80499a8:	cd 30                	int    $0x30
 80499aa:	83 c4 10             	add    $0x10,%esp
 80499ad:	5d                   	pop    %ebp
 80499ae:	c3                   	ret    

080499af <seek>:
 80499af:	55                   	push   %ebp
 80499b0:	89 e5                	mov    %esp,%ebp
 80499b2:	ff 75 0c             	pushl  0xc(%ebp)
 80499b5:	ff 75 08             	pushl  0x8(%ebp)
 80499b8:	6a 0a                	push   $0xa
 80499ba:	cd 30                	int    $0x30
 80499bc:	83 c4 0c             	add    $0xc,%esp
 80499bf:	5d                   	pop    %ebp
 80499c0:	c3                   	ret    

080499c1 <tell>:
 80499c1:	55                   	push   %ebp
 80499c2:	89 e5                	mov    %esp,%ebp
 80499c4:	ff 75 08             	pushl  0x8(%ebp)
 80499c7:	6a 0b                	push   $0xb
 80499c9:	cd 30                	int    $0x30
 80499cb:	83 c4 08             	add    $0x8,%esp
 80499ce:	5d                   	pop    %ebp
 80499cf:	c3                   	ret    

080499d0 <close>:
 80499d0:	55                   	push   %ebp
 80499d1:	89 e5                	mov    %esp,%ebp
 80499d3:	ff 75 08             	pushl  0x8(%ebp)
 80499d6:	6a 0c                	push   $0xc
 80499d8:	cd 30                	int    $0x30
 80499da:	83 c4 08             	add    $0x8,%esp
 80499dd:	5d                   	pop    %ebp
 80499de:	c3                   	ret    

080499df <mmap>:
 80499df:	55                   	push   %ebp
 80499e0:	89 e5                	mov    %esp,%ebp
 80499e2:	ff 75 0c             	pushl  0xc(%ebp)
 80499e5:	ff 75 08             	pushl  0x8(%ebp)
 80499e8:	6a 0d                	push   $0xd
 80499ea:	cd 30                	int    $0x30
 80499ec:	83 c4 0c             	add    $0xc,%esp
 80499ef:	5d                   	pop    %ebp
 80499f0:	c3                   	ret    

080499f1 <munmap>:
 80499f1:	55                   	push   %ebp
 80499f2:	89 e5                	mov    %esp,%ebp
 80499f4:	ff 75 08             	pushl  0x8(%ebp)
 80499f7:	6a 0e                	push   $0xe
 80499f9:	cd 30                	int    $0x30
 80499fb:	83 c4 08             	add    $0x8,%esp
 80499fe:	5d                   	pop    %ebp
 80499ff:	c3                   	ret    

08049a00 <chdir>:
 8049a00:	55                   	push   %ebp
 8049a01:	89 e5                	mov    %esp,%ebp
 8049a03:	ff 75 08             	pushl  0x8(%ebp)
 8049a06:	6a 0f                	push   $0xf
 8049a08:	cd 30                	int    $0x30
 8049a0a:	83 c4 08             	add    $0x8,%esp
 8049a0d:	85 c0                	test   %eax,%eax
 8049a0f:	0f 95 c0             	setne  %al
 8049a12:	0f b6 c0             	movzbl %al,%eax
 8049a15:	5d                   	pop    %ebp
 8049a16:	c3                   	ret    

08049a17 <mkdir>:
 8049a17:	55                   	push   %ebp
 8049a18:	89 e5                	mov    %esp,%ebp
 8049a1a:	ff 75 08             	pushl  0x8(%ebp)
 8049a1d:	6a 10                	push   $0x10
 8049a1f:	cd 30                	int    $0x30
 8049a21:	83 c4 08             	add    $0x8,%esp
 8049a24:	85 c0                	test   %eax,%eax
 8049a26:	0f 95 c0             	setne  %al
 8049a29:	0f b6 c0             	movzbl %al,%eax
 8049a2c:	5d                   	pop    %ebp
 8049a2d:	c3                   	ret    

08049a2e <readdir>:
 8049a2e:	55                   	push   %ebp
 8049a2f:	89 e5                	mov    %esp,%ebp
 8049a31:	ff 75 0c             	pushl  0xc(%ebp)
 8049a34:	ff 75 08             	pushl  0x8(%ebp)
 8049a37:	6a 11                	push   $0x11
 8049a39:	cd 30                	int    $0x30
 8049a3b:	83 c4 0c             	add    $0xc,%esp
 8049a3e:	85 c0                	test   %eax,%eax
 8049a40:	0f 95 c0             	setne  %al
 8049a43:	0f b6 c0             	movzbl %al,%eax
 8049a46:	5d                   	pop    %ebp
 8049a47:	c3                   	ret    

08049a48 <isdir>:
 8049a48:	55                   	push   %ebp
 8049a49:	89 e5                	mov    %esp,%ebp
 8049a4b:	ff 75 08             	pushl  0x8(%ebp)
 8049a4e:	6a 12                	push   $0x12
 8049a50:	cd 30                	int    $0x30
 8049a52:	83 c4 08             	add    $0x8,%esp
 8049a55:	85 c0                	test   %eax,%eax
 8049a57:	0f 95 c0             	setne  %al
 8049a5a:	0f b6 c0             	movzbl %al,%eax
 8049a5d:	5d                   	pop    %ebp
 8049a5e:	c3                   	ret    

08049a5f <inumber>:
 8049a5f:	55                   	push   %ebp
 8049a60:	89 e5                	mov    %esp,%ebp
 8049a62:	ff 75 08             	pushl  0x8(%ebp)
 8049a65:	6a 13                	push   $0x13
 8049a67:	cd 30                	int    $0x30
 8049a69:	83 c4 08             	add    $0x8,%esp
 8049a6c:	5d                   	pop    %ebp
 8049a6d:	c3                   	ret    

08049a6e <exit>:
 8049a6e:	55                   	push   %ebp
 8049a6f:	89 e5                	mov    %esp,%ebp
 8049a71:	83 ec 18             	sub    $0x18,%esp
 8049a74:	ff 75 08             	pushl  0x8(%ebp)
 8049a77:	6a 01                	push   $0x1
 8049a79:	cd 30                	int    $0x30
 8049a7b:	83 c4 08             	add    $0x8,%esp
 8049a7e:	c7 44 24 0c b8 a0 04 	movl   $0x804a0b8,0xc(%esp)
 8049a85:	08 
 8049a86:	c7 44 24 08 f4 9e 04 	movl   $0x8049ef4,0x8(%esp)
 8049a8d:	08 
 8049a8e:	c7 44 24 04 4b 00 00 	movl   $0x4b,0x4(%esp)
 8049a95:	00 
 8049a96:	c7 04 24 91 a0 04 08 	movl   $0x804a091,(%esp)
 8049a9d:	e8 1a fe ff ff       	call   80498bc <debug_panic>

08049aa2 <halt>:
 8049aa2:	55                   	push   %ebp
 8049aa3:	89 e5                	mov    %esp,%ebp
 8049aa5:	83 ec 18             	sub    $0x18,%esp
 8049aa8:	6a 00                	push   $0x0
 8049aaa:	cd 30                	int    $0x30
 8049aac:	83 c4 04             	add    $0x4,%esp
 8049aaf:	c7 44 24 0c b8 a0 04 	movl   $0x804a0b8,0xc(%esp)
 8049ab6:	08 
 8049ab7:	c7 44 24 08 f9 9e 04 	movl   $0x8049ef9,0x8(%esp)
 8049abe:	08 
 8049abf:	c7 44 24 04 44 00 00 	movl   $0x44,0x4(%esp)
 8049ac6:	00 
 8049ac7:	c7 04 24 91 a0 04 08 	movl   $0x804a091,(%esp)
 8049ace:	e8 e9 fd ff ff       	call   80498bc <debug_panic>
 8049ad3:	90                   	nop    

08049ad4 <flush>:
 8049ad4:	55                   	push   %ebp
 8049ad5:	89 e5                	mov    %esp,%ebp
 8049ad7:	83 ec 18             	sub    $0x18,%esp
 8049ada:	89 5d f8             	mov    %ebx,-0x8(%ebp)
 8049add:	89 75 fc             	mov    %esi,-0x4(%ebp)
 8049ae0:	89 c3                	mov    %eax,%ebx
 8049ae2:	8b 40 40             	mov    0x40(%eax),%eax
 8049ae5:	89 de                	mov    %ebx,%esi
 8049ae7:	39 d8                	cmp    %ebx,%eax
 8049ae9:	76 15                	jbe    8049b00 <flush+0x2c>
 8049aeb:	29 d8                	sub    %ebx,%eax
 8049aed:	89 44 24 08          	mov    %eax,0x8(%esp)
 8049af1:	89 5c 24 04          	mov    %ebx,0x4(%esp)
 8049af5:	8b 43 48             	mov    0x48(%ebx),%eax
 8049af8:	89 04 24             	mov    %eax,(%esp)
 8049afb:	e8 9a fe ff ff       	call   804999a <write>
 8049b00:	89 73 40             	mov    %esi,0x40(%ebx)
 8049b03:	8b 5d f8             	mov    -0x8(%ebp),%ebx
 8049b06:	8b 75 fc             	mov    -0x4(%ebp),%esi
 8049b09:	89 ec                	mov    %ebp,%esp
 8049b0b:	5d                   	pop    %ebp
 8049b0c:	c3                   	ret    

08049b0d <add_char>:
 8049b0d:	55                   	push   %ebp
 8049b0e:	89 e5                	mov    %esp,%ebp
 8049b10:	53                   	push   %ebx
 8049b11:	83 ec 04             	sub    $0x4,%esp
 8049b14:	8b 5d 0c             	mov    0xc(%ebp),%ebx
 8049b17:	8b 43 40             	mov    0x40(%ebx),%eax
 8049b1a:	8b 55 08             	mov    0x8(%ebp),%edx
 8049b1d:	88 10                	mov    %dl,(%eax)
 8049b1f:	83 c0 01             	add    $0x1,%eax
 8049b22:	89 43 40             	mov    %eax,0x40(%ebx)
 8049b25:	8d 53 40             	lea    0x40(%ebx),%edx
 8049b28:	39 d0                	cmp    %edx,%eax
 8049b2a:	72 07                	jb     8049b33 <add_char+0x26>
 8049b2c:	89 d8                	mov    %ebx,%eax
 8049b2e:	e8 a1 ff ff ff       	call   8049ad4 <flush>
 8049b33:	83 43 44 01          	addl   $0x1,0x44(%ebx)
 8049b37:	83 c4 04             	add    $0x4,%esp
 8049b3a:	5b                   	pop    %ebx
 8049b3b:	5d                   	pop    %ebp
 8049b3c:	c3                   	ret    

08049b3d <vhprintf>:
 8049b3d:	55                   	push   %ebp
 8049b3e:	89 e5                	mov    %esp,%ebp
 8049b40:	53                   	push   %ebx
 8049b41:	83 ec 64             	sub    $0x64,%esp
 8049b44:	8d 5d b0             	lea    -0x50(%ebp),%ebx
 8049b47:	89 5d f0             	mov    %ebx,-0x10(%ebp)
 8049b4a:	c7 45 f4 00 00 00 00 	movl   $0x0,-0xc(%ebp)
 8049b51:	8b 45 08             	mov    0x8(%ebp),%eax
 8049b54:	89 45 f8             	mov    %eax,-0x8(%ebp)
 8049b57:	89 5c 24 0c          	mov    %ebx,0xc(%esp)
 8049b5b:	c7 44 24 08 0d 9b 04 	movl   $0x8049b0d,0x8(%esp)
 8049b62:	08 
 8049b63:	8b 45 10             	mov    0x10(%ebp),%eax
 8049b66:	89 44 24 04          	mov    %eax,0x4(%esp)
 8049b6a:	8b 45 0c             	mov    0xc(%ebp),%eax
 8049b6d:	89 04 24             	mov    %eax,(%esp)
 8049b70:	e8 29 e9 ff ff       	call   804849e <__vprintf>
 8049b75:	89 d8                	mov    %ebx,%eax
 8049b77:	e8 58 ff ff ff       	call   8049ad4 <flush>
 8049b7c:	8b 45 f4             	mov    -0xc(%ebp),%eax
 8049b7f:	83 c4 64             	add    $0x64,%esp
 8049b82:	5b                   	pop    %ebx
 8049b83:	5d                   	pop    %ebp
 8049b84:	c3                   	ret    

08049b85 <vprintf>:
 8049b85:	55                   	push   %ebp
 8049b86:	89 e5                	mov    %esp,%ebp
 8049b88:	83 ec 18             	sub    $0x18,%esp
 8049b8b:	8b 45 0c             	mov    0xc(%ebp),%eax
 8049b8e:	89 44 24 08          	mov    %eax,0x8(%esp)
 8049b92:	8b 45 08             	mov    0x8(%ebp),%eax
 8049b95:	89 44 24 04          	mov    %eax,0x4(%esp)
 8049b99:	c7 04 24 01 00 00 00 	movl   $0x1,(%esp)
 8049ba0:	e8 98 ff ff ff       	call   8049b3d <vhprintf>
 8049ba5:	c9                   	leave  
 8049ba6:	c3                   	ret    

08049ba7 <putchar>:
 8049ba7:	55                   	push   %ebp
 8049ba8:	89 e5                	mov    %esp,%ebp
 8049baa:	53                   	push   %ebx
 8049bab:	83 ec 24             	sub    $0x24,%esp
 8049bae:	8b 5d 08             	mov    0x8(%ebp),%ebx
 8049bb1:	88 5d fb             	mov    %bl,-0x5(%ebp)
 8049bb4:	c7 44 24 08 01 00 00 	movl   $0x1,0x8(%esp)
 8049bbb:	00 
 8049bbc:	8d 45 fb             	lea    -0x5(%ebp),%eax
 8049bbf:	89 44 24 04          	mov    %eax,0x4(%esp)
 8049bc3:	c7 04 24 01 00 00 00 	movl   $0x1,(%esp)
 8049bca:	e8 cb fd ff ff       	call   804999a <write>
 8049bcf:	89 d8                	mov    %ebx,%eax
 8049bd1:	83 c4 24             	add    $0x24,%esp
 8049bd4:	5b                   	pop    %ebx
 8049bd5:	5d                   	pop    %ebp
 8049bd6:	c3                   	ret    

08049bd7 <puts>:
 8049bd7:	55                   	push   %ebp
 8049bd8:	89 e5                	mov    %esp,%ebp
 8049bda:	57                   	push   %edi
 8049bdb:	83 ec 14             	sub    $0x14,%esp
 8049bde:	8b 55 08             	mov    0x8(%ebp),%edx
 8049be1:	89 d7                	mov    %edx,%edi
 8049be3:	fc                   	cld    
 8049be4:	b9 ff ff ff ff       	mov    $0xffffffff,%ecx
 8049be9:	b8 00 00 00 00       	mov    $0x0,%eax
 8049bee:	f2 ae                	repnz scas %es:(%edi),%al
 8049bf0:	f7 d1                	not    %ecx
 8049bf2:	83 e9 01             	sub    $0x1,%ecx
 8049bf5:	89 4c 24 08          	mov    %ecx,0x8(%esp)
 8049bf9:	89 54 24 04          	mov    %edx,0x4(%esp)
 8049bfd:	c7 04 24 01 00 00 00 	movl   $0x1,(%esp)
 8049c04:	e8 91 fd ff ff       	call   804999a <write>
 8049c09:	c7 04 24 0a 00 00 00 	movl   $0xa,(%esp)
 8049c10:	e8 92 ff ff ff       	call   8049ba7 <putchar>
 8049c15:	b8 00 00 00 00       	mov    $0x0,%eax
 8049c1a:	83 c4 14             	add    $0x14,%esp
 8049c1d:	5f                   	pop    %edi
 8049c1e:	5d                   	pop    %ebp
 8049c1f:	c3                   	ret    

08049c20 <hprintf>:
 8049c20:	55                   	push   %ebp
 8049c21:	89 e5                	mov    %esp,%ebp
 8049c23:	83 ec 28             	sub    $0x28,%esp
 8049c26:	8d 45 10             	lea    0x10(%ebp),%eax
 8049c29:	89 45 fc             	mov    %eax,-0x4(%ebp)
 8049c2c:	89 44 24 08          	mov    %eax,0x8(%esp)
 8049c30:	8b 45 0c             	mov    0xc(%ebp),%eax
 8049c33:	89 44 24 04          	mov    %eax,0x4(%esp)
 8049c37:	8b 45 08             	mov    0x8(%ebp),%eax
 8049c3a:	89 04 24             	mov    %eax,(%esp)
 8049c3d:	e8 fb fe ff ff       	call   8049b3d <vhprintf>
 8049c42:	c9                   	leave  
 8049c43:	c3                   	ret    

08049c44 <debug_backtrace>:
 8049c44:	55                   	push   %ebp
 8049c45:	89 e5                	mov    %esp,%ebp
 8049c47:	53                   	push   %ebx
 8049c48:	83 ec 14             	sub    $0x14,%esp
 8049c4b:	c7 04 24 a7 a0 04 08 	movl   $0x804a0a7,(%esp)
 8049c52:	e8 c5 ee ff ff       	call   8048b1c <printf>
 8049c57:	89 eb                	mov    %ebp,%ebx
 8049c59:	83 7d 00 00          	cmpl   $0x0,0x0(%ebp)
 8049c5d:	74 20                	je     8049c7f <debug_backtrace+0x3b>
 8049c5f:	8b 43 04             	mov    0x4(%ebx),%eax
 8049c62:	89 44 24 04          	mov    %eax,0x4(%esp)
 8049c66:	c7 04 24 b3 a0 04 08 	movl   $0x804a0b3,(%esp)
 8049c6d:	e8 aa ee ff ff       	call   8048b1c <printf>
 8049c72:	8b 03                	mov    (%ebx),%eax
 8049c74:	85 c0                	test   %eax,%eax
 8049c76:	74 07                	je     8049c7f <debug_backtrace+0x3b>
 8049c78:	89 c3                	mov    %eax,%ebx
 8049c7a:	83 38 00             	cmpl   $0x0,(%eax)
 8049c7d:	75 e0                	jne    8049c5f <debug_backtrace+0x1b>
 8049c7f:	c7 04 24 b4 9f 04 08 	movl   $0x8049fb4,(%esp)
 8049c86:	e8 4c ff ff ff       	call   8049bd7 <puts>
 8049c8b:	80 3d a0 b1 04 08 00 	cmpb   $0x0,0x804b1a0
 8049c92:	75 13                	jne    8049ca7 <debug_backtrace+0x63>
 8049c94:	c6 05 a0 b1 04 08 01 	movb   $0x1,0x804b1a0
 8049c9b:	c7 04 24 04 a1 04 08 	movl   $0x804a104,(%esp)
 8049ca2:	e8 30 ff ff ff       	call   8049bd7 <puts>
 8049ca7:	83 c4 14             	add    $0x14,%esp
 8049caa:	5b                   	pop    %ebx
 8049cab:	5d                   	pop    %ebp
 8049cac:	c3                   	ret    
