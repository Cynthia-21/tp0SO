package ar.edu.utn.frba.dds.fuentedinamica.models.repositories.impl;

import ar.edu.utn.frba.dds.fuentedinamica.models.domain.fuentes.FuenteDinamica;
import ar.edu.utn.frba.dds.fuentedinamica.models.domain.fuentes.Fuente;
import ar.edu.utn.frba.dds.fuentedinamica.models.repositories.IFuenteDinamicaRepository;
import org.springframework.stereotype.Repository;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

@Repository
public class FuenteDinamicaRepository implements IFuenteDinamicaRepository {

  private List<FuenteDinamica> fuentesDinamicas;

  public FuenteDinamicaRepository(){
    this.fuentesDinamicas = new ArrayList<>();
  }
  @Override
  public FuenteDinamica save(FuenteDinamica fuenteDinamica) {
    fuenteDinamica.setId((long) this.fuentesDinamicas.size());
    this.fuentesDinamicas.add(fuenteDinamica);

    return fuenteDinamica;
  }

  @Override
  public List<FuenteDinamica> findAll() {
    return this.fuentesDinamicas;
  }

  @Override
  public FuenteDinamica findById(Long id) {
    return this.fuentesDinamicas.stream().filter(fuenteDinamica-> fuenteDinamica.getId().equals(id)).findFirst().orElse(null);
  }

  @Override
  public void delete(FuenteDinamica fuenteDinamica) {
    this.fuentesDinamicas.remove(fuenteDinamica);
  }
}