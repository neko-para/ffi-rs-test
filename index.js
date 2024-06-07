const { open } = require('ffi-rs')
const {
  unwrapPointer,
  load,
  DataType,
  funcConstructor,
  createPointer
} = require('ffi-rs')

open({
  library: 'test',
  path: 'build/test.dll'
})

const func = createPointer({
  paramsType: [
    funcConstructor({
      paramsType: [],
      retType: DataType.I32
    })
  ],
  paramsValue: [
    () => {
      console.log('begin js callback')
      const ret = load({
        library: 'test',
        funcName: 'simple_add',
        retType: DataType.I32,
        paramsType: [DataType.I32, DataType.I32],
        paramsValue: [1, 2]
      })
      console.log('end js callback', ret)
      return 123
    }
  ]
})

const func_logger = createPointer({
  paramsType: [
    funcConstructor({
      paramsType: [],
      retType: DataType.I32
    })
  ],
  paramsValue: [
    () => {
      console.log('begin js logger callback')
      console.log('end js logger callback')
      return 1
    }
  ]
})

load({
  library: 'test',
  funcName: 'call_cb',
  retType: DataType.Void,
  paramsType: [DataType.External, DataType.External],
  paramsValue: [...unwrapPointer(func), ...unwrapPointer(func_logger)]
  // runInNewThread: true
})
