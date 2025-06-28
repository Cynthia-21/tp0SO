package ar.edu.utn.frba.dds.servicioagregador.services.impl;

import ar.edu.utn.frba.dds.servicioagregador.models.domain.Categoria;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.Coleccion;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.Etiqueta;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.Hecho;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.fuentes.Fuente;
import ar.edu.utn.frba.dds.servicioagregador.models.dtos.input.HechoInputDTO;
import ar.edu.utn.frba.dds.servicioagregador.models.dtos.output.HechoOutputDTO;
import ar.edu.utn.frba.dds.servicioagregador.models.mappers.HechoDinamicoMapper;
import ar.edu.utn.frba.dds.servicioagregador.models.mappers.HechoEstaticoMapper;
import ar.edu.utn.frba.dds.servicioagregador.models.mappers.HechoProxyMapper;
import ar.edu.utn.frba.dds.servicioagregador.models.repositories.IHechoRepository;
import ar.edu.utn.frba.dds.servicioagregador.services.IHechoService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.web.reactive.function.client.WebClient;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.List;
import java.util.stream.Collectors;

@Service
public class HechoService implements IHechoService {
  @Autowired
  private IHechoRepository hechoRepository;
  @Autowired
  private HechoEstaticoMapper hechoEstaticoMapper;
  @Autowired
  private HechoDinamicoMapper hechoDinamicoMapper;
  @Autowired
  private HechoProxyMapper hechoProxyMapper;

  @Override
  public List<HechoOutputDTO> obtenerTodosLosHechos() {
    return hechoRepository.findAll().stream().map(this::hechoToDto).collect(Collectors.toList());
  }

  @Override
  public List<HechoOutputDTO> obtenerHechosDeFuente(Fuente fuente) {
    return fuente.importarHechos().stream().map(this::hechoToDto).collect(Collectors.toList());
  }

  @Override
  public List<Hecho> obtenerHechosRecientes(Fuente fuente) {
    return fuente.importarHechosRecientes();
  }

  private HechoOutputDTO hechoToDto(Hecho hecho) {
    HechoOutputDTO hechoOutputDTO = new HechoOutputDTO();
    hechoOutputDTO.setId(hecho.getId());
    hechoOutputDTO.setTitulo(hecho.getTitulo());
    hechoOutputDTO.setDescripcion(hecho.getDescripcion());
    hechoOutputDTO.setCategoriasId(hecho.getCategorias().stream().map(Categoria::getId).collect(Collectors.toList()));
    hechoOutputDTO.setLatitud(hecho.getUbicacion().getLatitud());
    hechoOutputDTO.setLongitud(hecho.getUbicacion().getLongitud());
    hechoOutputDTO.setFechaAcontecimiento(hecho.getFechaAcontecimiento());
    hechoOutputDTO.setFechaDeCarga(hecho.getFechaDeCarga());
    hechoOutputDTO.setEtiquetasId(hecho.getEtiquetas().stream().map(Etiqueta::getId).collect(Collectors.toList()));
    return hechoOutputDTO;
  }
}
